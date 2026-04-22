#include <sem/hir/hir_visitors.h>

static const char* _resolve_variable_name(symbol_id_t id, sym_table_t* smt) {
    variable_info_t vi;
    do {
        if (VRTB_get_info_id(id, &vi, &smt->v)) {
            id = vi.p_id;
        }
    } while (id != NO_SYMBOL_ID);
    return vi.name->body;
}

int HIRWLKR_visit_setpos_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    str_memcpy(&ctx->prev_location, &ctx->curr_location, sizeof(file_position_t));
    str_memcpy(&ctx->curr_location, &b->farg->storage.pos, sizeof(file_position_t));
    return 1;
}

int HIRWLKR_visit_phi_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    list_t* vars = (list_t*)mm_malloc(sizeof(list_t));
    if (!vars) return 0;
    list_init(vars);
    
    set_foreach (int_tuple_t* t, &b->targ->storage.set.h) {
        list_add(vars, (void*)t->y);
    }
    
    return map_put(&ctx->definitions, (long)b->sarg->storage.var.v_id, (void*)vars);
}

typedef struct {
    long const_value;
    char defined_value;
} defined_variable_t;

/*
1 - defined raw number or a constant
2 - defined variable
3 - overdefined variable (need to be resolved)
*/
static int _resolve_subject_value(hir_subject_t* s, sym_table_t* smt, defined_variable_t* out) {
    if (HIR_is_vartype(s->t)) {
        variable_info_t vi;
        if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v) || !vi.vdi.defined) return 0;
        else {
            out->const_value   = vi.vdi.definition;
            out->defined_value = vi.vdi.defined == DEFINED_VARIABLE ? 2 : 3;
        }
    }
    else {
        out->defined_value = 1;
        switch (HIR_is_defined_type(s->t)) {
            case 1: out->const_value = s->storage.num.value->to_llong(s->storage.num.value); break;
            case 2: out->const_value = s->storage.cnst.value; break;
            default: return 0;
        }
    }
    
    return 1;
}

static int _sparce_find_variable_define_location(hir_block_t* b, symbol_id_t v_id, file_position_t* loc) {
    int found = 0;
    while (b) {
        if (found && b->op == HIR_SETPOS) {
            loc->line   = b->farg->storage.pos.line;
            loc->column = b->farg->storage.pos.column;
            loc->file   = b->farg->storage.pos.file;
            break;
        }
        else { 
            if (
                b->op != HIR_PHI_PREAMBLE &&
                (
                    (b->op == HIR_PHI && b->sarg->storage.var.v_id == v_id) ||
                    (b->farg && HIR_is_vartype(b->farg->t) && b->farg->storage.var.v_id == v_id)
                )
            ) found = 1;
        }

        b = b->prev;
    }

    return found;
}

static inline symbol_id_t _get_parent_id(symbol_id_t v_id, sym_table_t* smt) {
    variable_info_t vi;
    if (VRTB_get_info_id(v_id, &vi, &smt->v)) return vi.p_id;
    return NO_SYMBOL_ID;
}

static int _dereference_error(hir_block_t* hb, hir_subject_t* s, sym_table_t* smt, hir_visitors_ctx_t* ctx) {
    defined_variable_t di;
    if (!_resolve_subject_value(s, smt, &di)) {
        return 1;
    }

    queue_t work_vars;
    queue_init(&work_vars);

    trace_t trace;
    TRACE_init_trace(&trace);

    switch (di.defined_value) {
        /* Number defined   */
        case 1: {
            if (!di.const_value) TRACE_add_location(&trace, &ctx->curr_location, "NULL-dereference error!");
            break;
        }
        /* Variable defined */
        case 2: {
            if (!di.const_value) {
                file_position_t loc;
                _sparce_find_variable_define_location(hb, s->storage.var.v_id, &loc);
                TRACE_add_location(
                    &trace, &loc, "Variable '%s' is assigned with NULL here", 
                    _resolve_variable_name(s->storage.var.v_id, smt)
                );
                TRACE_add_location(&trace, &ctx->curr_location,
                    "NULL-dereference error (variable '%s' is NULL)!", 
                    _resolve_variable_name(s->storage.var.v_id, smt)
                );
            }

            break;
        }
        /* Overdefined */
        case 3: queue_push(&work_vars, (void*)di.const_value);
        default: break;
    }

    if (queue_isempty(&work_vars)) {
        TRACE_print_and_free_trace(&trace);
        queue_free(&work_vars);
        return 1;
    }

    int res = 1;
    symbol_id_t v_id, prev_id = s->storage.var.v_id;
    while (queue_pop(&work_vars, (void**)&v_id)) {
        list_t* possible_definitions;
        if (map_get(&ctx->definitions, (long)v_id, (void**)&possible_definitions)) {
            file_position_t loc;
            _sparce_find_variable_define_location(hb, prev_id, &loc);
            if (
                _get_parent_id(prev_id, smt) != _get_parent_id(v_id, smt)
            ) TRACE_add_location(
                &trace, &loc, "Variable '%s' is assigned with the '%s' here", 
                _resolve_variable_name(prev_id, smt), _resolve_variable_name(v_id, smt)
            );

            foreach (symbol_id_t p_id, possible_definitions) {
                queue_push(&work_vars, (void*)p_id);
            }
        }

        variable_info_t vi;
        if (!VRTB_get_info_id(v_id, &vi, &smt->v) || !vi.vdi.defined) continue;
        else {
            if (vi.vdi.defined == OVERDEFINED_VARIABLE && vi.vdi.definition != vi.v_id) {
                queue_push(&work_vars, (void*)vi.vdi.definition);
                prev_id = v_id;
            }
            else {
                if (!vi.vdi.definition) {
                    res = 0;
                    file_position_t loc;
                    _sparce_find_variable_define_location(hb, vi.v_id, &loc);
                    TRACE_add_location(&trace, &loc, "Variable '%s' becomes NULL-value", vi.name->body);
                }
            }
        }
    }

    if (res) TRACE_unload_trace(&trace);
    else {
        TRACE_add_location(
            &trace, &ctx->curr_location, 
            "Possible NULL-dereference error (variable '%s' is NULL)!", 
            _resolve_variable_name(s->storage.var.v_id, smt)
        );

        TRACE_print_and_free_trace(&trace);
    }

    queue_free(&work_vars);
    return 1;
}

int HIRWLKR_visit_gdref_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    if (b->op == HIR_SYSC || b->op == HIR_STORE_SYSC) return 1;
    return _dereference_error(b, b->sarg, smt, ctx);
}

int HIRWLKR_visit_ldref_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    if (b->op == HIR_SYSC || b->op == HIR_STORE_SYSC) return 1;
    return _dereference_error(b, b->farg, smt, ctx);
}

int HIRWLKR_visit_ifop2_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;

    trace_t trace;
    TRACE_init_trace(&trace);

    defined_variable_t di;
    if (!_resolve_subject_value(b->farg, smt, &di)) {
        return 1;
    }

    switch (di.defined_value) {
        case 1: TRACE_add_location(&trace, &ctx->curr_location, "'If' with a constant value '%s'!", di.const_value ? "true" : "false"); break;
        case 2: {
            file_position_t loc;
            _sparce_find_variable_define_location(b, b->farg->storage.var.v_id, &loc);
            TRACE_add_location(&trace, &loc, "Variable '%s' declared as a constant here!", _resolve_variable_name(b->farg->storage.var.v_id, smt));
            TRACE_add_location(
                &trace, &ctx->curr_location, "Condition with a constant value (variable '%s' is equals '%s' (%i))!", 
                _resolve_variable_name(b->farg->storage.var.v_id, smt), di.const_value ? "true" : "false", di.const_value
            ); 
            break;
        }
        default: break;
    }

    TRACE_print_and_free_trace(&trace);
    return 1;
}

static int _create_type_name(hir_subject_type_t t, int ptr, char* buffer, int buffer_size) {
    for (int i = 0; i < ptr; i++) {
        buffer += snprintf(buffer, buffer_size, "ptr ");
    }

    switch (t) {
        case HIR_STKVARSTR:
        case HIR_TMPVARSTR:
        case HIR_GLBVARSTR:  buffer += snprintf(buffer, buffer_size, "str"); break;
        case HIR_GLBVARARR:
        case HIR_STKVARARR:
        case HIR_TMPVARARR:  buffer += snprintf(buffer, buffer_size, "arr"); break;
        case HIR_STKVARF64:
        case HIR_TMPVARF64:
        case HIR_F64NUMBER:
        case HIR_GLBVARF64:  buffer += snprintf(buffer, buffer_size, "f64"); break;
        case HIR_STKVARU64:
        case HIR_TMPVARU64:
        case HIR_U64NUMBER:
        case HIR_GLBVARU64:  buffer += snprintf(buffer, buffer_size, "u64"); break;
        case HIR_STKVARI64:
        case HIR_TMPVARI64:
        case HIR_I64NUMBER:
        case HIR_GLBVARI64:  buffer += snprintf(buffer, buffer_size, "i64"); break;
        case HIR_STKVARF32:
        case HIR_TMPVARF32:
        case HIR_F32NUMBER:
        case HIR_GLBVARF32:  buffer += snprintf(buffer, buffer_size, "f32"); break;  
        case HIR_STKVARU32:
        case HIR_TMPVARU32:
        case HIR_U32NUMBER:
        case HIR_GLBVARU32:  buffer += snprintf(buffer, buffer_size, "u32"); break;
        case HIR_STKVARI32:
        case HIR_TMPVARI32:
        case HIR_I32NUMBER:
        case HIR_GLBVARI32:  buffer += snprintf(buffer, buffer_size, "i32"); break;
        case HIR_STKVARU16:
        case HIR_GLBVARU16:
        case HIR_U16NUMBER:  
        case HIR_TMPVARU16:  buffer += snprintf(buffer, buffer_size, "u16"); break;
        case HIR_GLBVARI16:
        case HIR_TMPVARI16: 
        case HIR_I16NUMBER:
        case HIR_STKVARI16:  buffer += snprintf(buffer, buffer_size, "i16"); break;
        case HIR_GLBVARU8: 
        case HIR_STKVARU8: 
        case HIR_U8NUMBER:
        case HIR_TMPVARU8:   buffer += snprintf(buffer, buffer_size, "u8");  break;
        case HIR_STKVARI8:
        case HIR_GLBVARI8:
        case HIR_I8NUMBER:
        case HIR_TMPVARI8:   buffer += snprintf(buffer, buffer_size, "i8");  break;
        case HIR_STKVARI0: 
        case HIR_TMPVARI0:
        case HIR_GLBVARI0:   buffer += snprintf(buffer, buffer_size, "i0");  break;
        case HIR_NUMBER:     buffer += snprintf(buffer, buffer_size, "num"); break;
        case HIR_U8CONSTVAL:  case HIR_I8CONSTVAL:
        case HIR_U16CONSTVAL: case HIR_I16CONSTVAL:
        case HIR_U32CONSTVAL: case HIR_I32CONSTVAL:
        case HIR_U64CONSTVAL: case HIR_I64CONSTVAL:
            buffer += snprintf(buffer, buffer_size, "cnt"); break;
        default: break;
    }

    return 1;
}

int HIRWLKR_wrong_arg_type(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    if (b->op == HIR_SYSC || b->op == HIR_STORE_SYSC) return 1;
    func_info_t fi;
    if (!FNTB_get_info_id(b->sarg->storage.str.s_id, &fi, &smt->f)) {
        return 1;
    }

    trace_t trace;
    TRACE_init_trace(&trace);

    int arg_index = 0;
    hir_subject_t** hir_args = (hir_subject_t**)list_flatten(&b->targ->storage.list.h);
    fn_iterate_args (&fi) {
        if (
            HIR_get_tmptype_tkn(arg->t, 0) != hir_args[arg_index]->t ||
            arg->t->flags.ptr != hir_args[arg_index]->ptr
        ) {
            char received[64], expected[64];
            _create_type_name(HIR_get_tmptype_tkn(arg->t, 0), arg->t->flags.ptr, expected, sizeof(expected));
            _create_type_name(hir_args[arg_index]->t, hir_args[arg_index]->ptr, received, sizeof(received));

            if (HIR_is_defined_type(hir_args[arg_index]->t)) {
                defined_variable_t di;
                if (!_resolve_subject_value(hir_args[arg_index], smt, &di)) continue;
                TRACE_add_location(
                    &trace, &ctx->curr_location, 
                    "Value '%ld' has the '%s' type! Consider the 'as %s' command!", di.const_value, received, expected
                );
            }
            else {
                variable_info_t vi;
                if (!VRTB_get_info_id(hir_args[arg_index]->storage.var.v_id, &vi, &smt->v)) continue;
                TRACE_add_location(
                    &trace, &ctx->curr_location, 
                    "Variable '%s' has the '%s' type! Consider the 'as %s' command!", vi.name->body, received, expected
                );

                file_position_t loc;
                _sparce_find_variable_define_location(b, hir_args[arg_index]->storage.var.v_id, &loc);
                TRACE_add_location(&trace, &loc, "Variable '%s' declared here!", vi.name->body);
            }

        }
        
        arg_index++;
    }

    if (!TRACE_is_empty(&trace)) {
        TRACE_add_location(
            &trace, &ctx->curr_location, 
            "Function '%s' has some arguments, which have the wrong type! Consider to use the 'as' operator!", fi.name->body
        );
    }

    mm_free(hir_args);
    TRACE_print_and_free_trace(&trace);
    return 1;
}

// TODO: Implement a visitor which detects for dangerous syscalls
int HIRWLKR_visit_syscall_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    if (b->op != HIR_SYSC && b->op != HIR_STORE_SYSC) return 1;
    hir_subject_t* number = list_get_head(&b->targ->storage.list.h);
    defined_variable_t di;
    if (!_resolve_subject_value(number, smt, &di) || di.defined_value == 3) {
        return 1;
    }

    trace_t trace;
    TRACE_init_trace(&trace);

    int table_size = -1;
    syscall_t* table = NULL;
    hir_subject_t** flatten_input = (hir_subject_t**)list_flatten(&b->targ->storage.list.h);
    switch (CONF_get_system_type()) {
        case MACOH64: {
            table_size = SYSCHECK_get_macoh_x86_64_syscall_table(&table);
            /* MacOS syscall offset */
            di.const_value -= 0x2000000;
            break;
        }
        case LINUX64: table_size = SYSCHECK_get_linux_x86_64_syscall_table(&table); break;
        default: goto _force_exit_syscall_checker;
    }

    if (di.const_value >= table_size || di.const_value < 0) {
        TRACE_add_location(&trace, &ctx->curr_location, "Selected architecture doesn't have a syscall for %li value!", di.const_value);
        if (di.defined_value == 2) {
            file_position_t loc;
            _sparce_find_variable_define_location(b, number->storage.var.v_id, &loc);
            TRACE_add_location(
                &trace, &loc, "Variable '%s' is assigned with this value here", 
                _resolve_variable_name(number->storage.var.v_id, smt)
            );
        }
        goto _force_exit_syscall_checker;
    }

    syscall_t syscall = table[di.const_value];
    for (int arg_index = 1; arg_index < syscall.argc && arg_index < list_size(&b->targ->storage.list.h); arg_index++) {
        int sarg_index = arg_index - 1;
        if (
            HIR_get_tmp_type(flatten_input[arg_index]->t) != syscall.types[sarg_index].t ||
            flatten_input[arg_index]->ptr != syscall.types[sarg_index].ptr
        ) {
            char received[64] = { 0 }, expected[64] = { 0 };
            _create_type_name(syscall.types[sarg_index].t, syscall.types[sarg_index].ptr, expected, sizeof(expected));
            _create_type_name(flatten_input[arg_index]->t, flatten_input[arg_index]->ptr, received, sizeof(received));
            TRACE_add_location(
                &trace, &ctx->curr_location, 
                "%i argument (%s, %s) should have the '%s' type, but the '%s' is provided! Consider to cast it with 'as %s'.", 
                arg_index + 1, syscall.types[sarg_index].name, syscall.types[sarg_index].description, expected, received, expected
            );

            if (!HIR_is_defined_type(flatten_input[arg_index]->t)) {
                file_position_t loc;
                _sparce_find_variable_define_location(b, flatten_input[arg_index]->storage.var.v_id, &loc);
                TRACE_add_location(&trace, &loc, "The variable is defined here!");
            }

        }
        
        if (syscall.types[sarg_index].dereference) {
            _dereference_error(b, flatten_input[arg_index], smt, ctx);
        }
    }

    if (!TRACE_is_empty(&trace)) {
        TRACE_add_location(
            &trace, &ctx->curr_location, 
            "Syscall (%s, %s) with number %i has some wrong typed arguments! It can lead to UB, consider to cast them:", 
            syscall.name, syscall.description, di.const_value
        );
    }

_force_exit_syscall_checker: {}
    mm_free(flatten_input);
    TRACE_print_and_free_trace(&trace);
    return 1;
}

int HIRWLKR_unused_rtype(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;

    func_info_t fi;
    if (
        (!b->sarg || b->sarg->t != HIR_FNAME) || 
        !FNTB_get_info_id(b->sarg->storage.str.s_id, &fi, &smt->f)
    ) return 1;
    
    if (
        fi.rtype && fi.rtype->t->t_type != I0_TYPE_TOKEN
    ) {
        trace_t trace;
        TRACE_init_trace(&trace);

        char rtype[64];
        _create_type_name(HIR_get_tmptype_tkn(fi.rtype->t, 0), fi.rtype->t->flags.ptr, rtype, sizeof(rtype));
        TRACE_add_location(
            &trace, &ctx->curr_location, 
            "Function '%s' has the '%s' return type but the call doesn't store it anywhere else.",
            fi.name->body, rtype
        );

        TRACE_print_and_free_trace(&trace);
        return 0;
    }

    return 1;
}

int HIRWLKR_noret_assign(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;

    func_info_t fi;
    if (
        b->sarg->t != HIR_FNAME || 
        !FNTB_get_info_id(b->sarg->storage.str.s_id, &fi, &smt->f)
    ) return 1;

    if (
        !fi.rtype || fi.rtype->t->t_type == I0_TYPE_TOKEN
    ) {
        trace_t trace;
        TRACE_init_trace(&trace);


        TRACE_add_location(
            &trace, &ctx->curr_location, 
            "Function '%s' doesn't return any value, but it used as a value. Consider to change the return type.",
            fi.name->body
        );

        TRACE_print_and_free_trace(&trace);
        return 0;
    }

    return 1;
}

int HIRWLKR_unused_expression(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    return 1;
}

int HIRWLKR_ref_to_expression(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;

    trace_t trace;
    TRACE_init_trace(&trace);

    if (HIR_is_tmptype(b->sarg->t)) {
        TRACE_add_location(
            &trace, &ctx->curr_location, 
            "The danger reference to a temp value! Consider to reference from a variable with this value."
        );
    }

    TRACE_print_and_free_trace(&trace);
    return 1;
}

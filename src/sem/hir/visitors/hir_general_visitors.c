#include <sem/hir/hir_visitors.h>

/* Resolve the original variable name by walking through parent variable ids.
Params:
    - `id` - Variable symbol id.
    - `smt` - Symtable.

Returns variable name if it was found. Otherwise returns "no-name" */
static const char* _resolve_variable_name(symbol_id_t id, sym_table_t* smt) {
    variable_info_t vi;
    do {
        if (VRTB_get_info_id(id, &vi, &smt->v)) id = vi.p_id;
        else return "no-name";
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
    long long const_value;
    char      defined_value;
} defined_variable_t;

/* 1 - defined raw number or a constant
2 - defined variable
3 - overdefined variable (need to be resolved) */
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

static int _subject_is_statically_known_not_value(hir_subject_t* s, sym_table_t* smt, long long value) {
    defined_variable_t di;
    if (!_resolve_subject_value(s, smt, &di)) return 0;
    return (di.defined_value == 1 || di.defined_value == 2) && di.const_value != value;
}

/* Find a source location where the variable was defined by scanning backwards
from the provided HIR block.
Params:
    - `b` - HIR block to start scanning from.
    - `v_id` - Variable symbol id.
    - `loc` - Output source location.

Returns 1 if variable definition was found, otherwise 0 */
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

static file_position_t _find_variable_define_location_or_current(
    hir_block_t* b, symbol_id_t v_id, file_position_t* current
) {
    file_position_t loc;
    str_memcpy(&loc, current, sizeof(file_position_t));
    _sparce_find_variable_define_location(b, v_id, &loc);
    return loc;
}

static const char* _value_name_or_numeric(long long value, const char* value_name, char* buffer, int buffer_size) {
    if (value_name) return value_name;
    snprintf(buffer, buffer_size, "'%lli'", value);
    return buffer;
}

typedef struct {
    file_position_t location;
    string_t*       message;
} pending_trace_note_t;

static int _unload_pending_trace_note(pending_trace_note_t* note) {
    if (!note) return 1;
    destroy_string(note->message);
    mm_free(note);
    return 1;
}

static int _add_pending_note(list_t* notes, file_position_t* loc, const char* fmt, ...) {
    char buffer[512] = { 0 };
    va_list args;
    va_start(args, fmt);
    vsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    pending_trace_note_t* note = (pending_trace_note_t*)mm_malloc(sizeof(pending_trace_note_t));
    if (!note) return 0;
    str_memcpy(&note->location, loc, sizeof(file_position_t));
    note->message = create_string(buffer);
    if (!note->message) {
        mm_free(note);
        return 0;
    }

    if (!list_add(notes, note)) {
        _unload_pending_trace_note(note);
        return 0;
    }

    return 1;
}

static int _attach_pending_notes(trace_t* trace, trace_id_t id, list_t* notes) {
    foreach (pending_trace_note_t* note, notes) {
        TRACE_add_note(trace, id, &note->location, "%s", note->message->body);
    }

    return 1;
}

/* Get parent variable symbol id.
Params:
    - `v_id` - Variable symbol id.
    - `smt` - Symtable.

Returns parent variable symbol id if it was found. Otherwise returns NO_SYMBOL_ID. */
static inline symbol_id_t _get_parent_id(symbol_id_t v_id, sym_table_t* smt) {
    variable_info_t vi;
    if (VRTB_get_info_id(v_id, &vi, &smt->v)) return vi.p_id;
    return NO_SYMBOL_ID;
}

int HIR_SEM_check_subject_value_and_provide_trace_ex(
    hir_block_t* hb, cfg_block_t* bb, hir_subject_t* s, sym_table_t* smt, hir_visitors_ctx_t* ctx,
    long long value, const char* value_name, hir_value_trace_mode_t mode, const char* error
) {
    if (!s) return 1;
    if (HIR_is_arrtype(s->t) || s->t == HIR_STRING) return 1;

    defined_variable_t di;
    if (!_resolve_subject_value(s, smt, &di)) return 1;

    char value_buffer[32];
    const char* value_repr = _value_name_or_numeric(value, value_name, value_buffer, sizeof(value_buffer));

    queue_t work_vars;
    queue_init(&work_vars);

    trace_t trace;
    TRACE_init_trace(&trace);

    list_t pending_notes;
    list_init(&pending_notes);

    switch (di.defined_value) {
        /* Number defined   */
        case 1: {
            if (di.const_value == value) {
                TRACE_create_root(&trace, TRACE_SEVERITY_WARNING, &ctx->curr_location, "%s", error);
                TRACE_print_and_free_trace(&trace);
                list_free_force_op(&pending_notes, (int (*)(void*))_unload_pending_trace_note);
                queue_free(&work_vars);
                return 0;
            }

            TRACE_unload_trace(&trace);
            list_free_force_op(&pending_notes, (int (*)(void*))_unload_pending_trace_note);
            queue_free(&work_vars);
            return 1;
        }
        /* Variable defined */
        case 2: {
            if (di.const_value != value) {
                TRACE_unload_trace(&trace);
                list_free_force_op(&pending_notes, (int (*)(void*))_unload_pending_trace_note);
                queue_free(&work_vars);
                return 1;
            }

            file_position_t loc = _find_variable_define_location_or_current(hb, s->storage.var.v_id, &ctx->curr_location);
            trace_id_t trace_id = TRACE_create_root(&trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
                "%s (variable '%s' is %s)!",
                error, _resolve_variable_name(s->storage.var.v_id, smt), value_repr
            );
            TRACE_add_note(
                &trace, trace_id, &loc, "Variable '%s' is assigned with %s here",
                _resolve_variable_name(s->storage.var.v_id, smt), value_repr
            );
            TRACE_print_and_free_trace(&trace);
            list_free_force_op(&pending_notes, (int (*)(void*))_unload_pending_trace_note);
            queue_free(&work_vars);
            return 0;
        }
        /* Overdefined */
        case 3: {
            queue_push(&work_vars, (void*)di.const_value);
            break;
        }
        default: break;
    }

    int has_static_possible_match = 0;
    if (!queue_isempty(&work_vars)) {
        symbol_id_t v_id, prev_id = s->storage.var.v_id;
        while (queue_pop(&work_vars, (void**)&v_id)) {
            list_t* possible_definitions;
            if (map_get(&ctx->definitions, (long)v_id, (void**)&possible_definitions)) {
                file_position_t loc = _find_variable_define_location_or_current(hb, prev_id, &ctx->curr_location);
                if (
                    _get_parent_id(prev_id, smt) != _get_parent_id(v_id, smt)
                ) _add_pending_note(
                    &pending_notes, &loc, "Variable '%s' is assigned with '%s' here",
                    _resolve_variable_name(prev_id, smt), _resolve_variable_name(v_id, smt)
                );

                foreach (symbol_id_t p_id, possible_definitions) {
                    queue_push(&work_vars, (void*)p_id);
                }
            }

            variable_info_t vi;
            if (!VRTB_get_info_id(v_id, &vi, &smt->v) || vi.vdi.defined == UNDEFINED_VARIABLE) continue;
            else {
                if (vi.vdi.defined == OVERDEFINED_VARIABLE && vi.vdi.definition != vi.v_id) {
                    queue_push(&work_vars, (void*)vi.vdi.definition);
                    prev_id = v_id;
                }
                else if (vi.vdi.defined == DEFINED_VARIABLE && vi.vdi.definition == value) {
                    has_static_possible_match = 1;
                    file_position_t loc = _find_variable_define_location_or_current(hb, vi.v_id, &ctx->curr_location);
                    _add_pending_note(&pending_notes, &loc, "Variable '%s' becomes %s", vi.name->body, value_repr);
                }
            }
        }
    }

    int z3_answer = Z3_check_subject_eq_llong_at_block(ctx->z3, bb->pfunc, bb, s, value);
    int should_report = mode == HIR_VALUE_TRACE_EXACT
        ? z3_answer == Z3A_YES
        : has_static_possible_match || z3_answer == Z3A_YES || z3_answer == Z3A_MAYBE;

    if (!should_report) TRACE_unload_trace(&trace);
    else {
        trace_id_t trace_id = TRACE_create_root(
            &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
            "%s%s (variable '%s' is %s)!",
            z3_answer == Z3A_MAYBE ? "Possible " : "", error,
            _resolve_variable_name(s->storage.var.v_id, smt), value_repr
        );
        _attach_pending_notes(&trace, trace_id, &pending_notes);

        TRACE_print_and_free_trace(&trace);
    }

    list_free_force_op(&pending_notes, (int (*)(void*))_unload_pending_trace_note);
    queue_free(&work_vars);
    return 1;
}

int HIR_SEM_check_subject_value_and_provide_trace(
    hir_block_t* hb, cfg_block_t* bb, hir_subject_t* s, sym_table_t* smt, hir_visitors_ctx_t* ctx,
    long long value, const char* error
) {
    return HIR_SEM_check_subject_value_and_provide_trace_ex(
        hb, bb, s, smt, ctx, value, NULL, HIR_VALUE_TRACE_POSSIBLE, error
    );
}

int HIRWLKR_visit_gdref_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    if (b->op == HIR_SYSC || b->op == HIR_STORE_SYSC) return 1;
    func_info_t fi;
    if (!FNTB_get_info_id(bb->pfunc->f_id, &fi, &smt->f)) {
        return 1;
    }

    return HIR_SEM_check_subject_value_and_provide_trace_ex(
        b, bb, b->sarg, smt, ctx, 0, "NULL", HIR_VALUE_TRACE_POSSIBLE, "NULL-dereference error"
    );
}

int HIRWLKR_visit_ldref_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    if (b->op == HIR_SYSC || b->op == HIR_STORE_SYSC) return 1;
    func_info_t fi;
    if (!FNTB_get_info_id(bb->pfunc->f_id, &fi, &smt->f)) {
        return 1;
    }

    return HIR_SEM_check_subject_value_and_provide_trace_ex(
        b, bb, b->farg, smt, ctx, 0, "NULL", HIR_VALUE_TRACE_POSSIBLE, "NULL-dereference error"
    );
}

int HIRWLKR_visit_ifop2_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;

    func_info_t fi;
    if (!FNTB_get_info_id(bb->pfunc->f_id, &fi, &smt->f)) {
        return 1;
    }

    trace_t trace;
    TRACE_init_trace(&trace);
    int then_reachable = bb->l
        ? Z3_is_edge_feasible(ctx->z3, bb->pfunc, bb, bb->l)
        : Z3_is_label_reachable(ctx->z3, bb->pfunc, b->sarg->id);
    int else_reachable = bb->jmp
        ? Z3_is_edge_feasible(ctx->z3, bb->pfunc, bb, bb->jmp)
        : Z3_is_label_reachable(ctx->z3, bb->pfunc, b->targ->id);
    if (!then_reachable && !else_reachable) {
        then_reachable = 1;
        else_reachable = 1;
    }

    if (!then_reachable) {
        TRACE_create_root(
            &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
            "Can't reach the 'then' branch! Consider to refactor the code."
        );
    }

    if (!else_reachable) {
        TRACE_create_root(
            &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
            "Can't reach the 'else' branch! Consider to refactor the code."
        );
    }

    defined_variable_t di;
    if (_resolve_subject_value(b->farg, smt, &di)) switch (di.defined_value) {
        case 1: {
            TRACE_create_root(
                &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
                "'If' with a constant value '%s'!", di.const_value ? "true" : "false"
            );
            break;
        }
        case 2: {
            file_position_t loc;
            _sparce_find_variable_define_location(b, b->farg->storage.var.v_id, &loc);
            trace_id_t trace_id = TRACE_create_root(
                &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
                "Condition with a constant value (variable '%s' is equals '%s' (%i))!",
                _resolve_variable_name(b->farg->storage.var.v_id, smt), di.const_value ? "true" : "false", di.const_value
            );
            TRACE_add_note(
                &trace, trace_id, &loc, "Variable '%s' declared as a constant here!",
                _resolve_variable_name(b->farg->storage.var.v_id, smt)
            );
            break;
        }
        default: break;
    }

    TRACE_print_and_free_trace(&trace);
    return 1;
}

/* Create a readable type name for a HIR subject type.
Params:
    - `t` - HIR subject type.
    - `ptr` - Pointer indirection level.
    - `buffer` - Output buffer.
    - `buffer_size` - Output buffer size.

Returns 1 on success, otherwise 0 */
static int _create_type_name(hir_subject_type_t t, int ptr, char* buffer, int buffer_size) {
    for (int i = 0; i < ptr; i++) {
        buffer += snprintf(buffer, buffer_size, "ptr ");
    }

    switch (t) {
        case HIR_GLBVARARR: case HIR_STKVARARR:
        case HIR_TMPVARARR:  buffer += snprintf(buffer, buffer_size, "arr"); break;
        case HIR_STKVARF64: case HIR_TMPVARF64: case HIR_F64NUMBER:
        case HIR_GLBVARF64:  buffer += snprintf(buffer, buffer_size, "f64"); break;
        case HIR_STKVARU64: case HIR_TMPVARU64: case HIR_U64NUMBER:
        case HIR_GLBVARU64:  buffer += snprintf(buffer, buffer_size, "u64"); break;
        case HIR_STKVARI64: case HIR_TMPVARI64: case HIR_I64NUMBER:
        case HIR_GLBVARI64:  buffer += snprintf(buffer, buffer_size, "i64"); break;
        case HIR_STKVARF32: case HIR_TMPVARF32: case HIR_F32NUMBER:
        case HIR_GLBVARF32:  buffer += snprintf(buffer, buffer_size, "f32"); break;
        case HIR_STKVARU32: case HIR_TMPVARU32: case HIR_U32NUMBER:
        case HIR_GLBVARU32:  buffer += snprintf(buffer, buffer_size, "u32"); break;
        case HIR_STKVARI32: case HIR_TMPVARI32: case HIR_I32NUMBER:
        case HIR_GLBVARI32:  buffer += snprintf(buffer, buffer_size, "i32"); break;
        case HIR_STKVARU16: case HIR_GLBVARU16: case HIR_U16NUMBER:
        case HIR_TMPVARU16:  buffer += snprintf(buffer, buffer_size, "u16"); break;
        case HIR_GLBVARI16: case HIR_TMPVARI16: case HIR_I16NUMBER:
        case HIR_STKVARI16:  buffer += snprintf(buffer, buffer_size, "i16"); break;
        case HIR_GLBVARU8:  case HIR_STKVARU8:  case HIR_U8NUMBER:
        case HIR_TMPVARU8:   buffer += snprintf(buffer, buffer_size, "u8");  break;
        case HIR_STKVARI8:  case HIR_GLBVARI8:  case HIR_I8NUMBER:
        case HIR_TMPVARI8:   buffer += snprintf(buffer, buffer_size, "i8");  break;
        case HIR_STKVARI0:  case HIR_TMPVARI0:
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

static inline int _compare_expected_with_provided(ast_node_t* expected, hir_subject_t* provided, sym_table_t* smt) {
    if (expected->t->flags.ptr != provided->ptr) return 0;
    if (expected->t->flags.ptr > 0) return 1;
    if (expected->t->t_type != CUSTOM_TYPE_TOKEN) {
        return HIR_get_type_size(HIR_get_tmptype_tkn(expected->t, 0)) ==
               HIR_get_type_size(HIR_get_tmp_type(provided->t));
    }

    variable_info_t pvi;
    if (
        HIR_is_vartype(provided->t) &&
        VRTB_get_info_id(provided->storage.var.v_id, &pvi, &smt->v)
    ) return TPTB_resolve_parent(expected->sinfo.t_id, &smt->t) == TPTB_resolve_parent(pvi.t_id, &smt->t);
    return 0;
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
    if (!hir_args) return 1;

    trace_id_t trace_id = TRACE_NO_ID;
    fn_iterate_args (&fi) {
        if (arg->t->t_type == VAR_ARGUMENTS_TOKEN) continue;
        if (!_compare_expected_with_provided(arg, hir_args[arg_index], smt)) {
            char received[64], expected[64];
            _create_type_name(HIR_get_tmptype_tkn(arg->t, 0), arg->t->flags.ptr, expected, sizeof(expected));
            _create_type_name(hir_args[arg_index]->t, hir_args[arg_index]->ptr, received, sizeof(received));

            if (trace_id == TRACE_NO_ID) {
                trace_id = TRACE_create_root(
                    &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
                    "Function '%s' has some arguments, which have a wrong type! Consider to use the 'as' operator!",
                    fi.name->body
                );
            }

            if (HIR_is_defined_type(hir_args[arg_index]->t)) {
                defined_variable_t di;
                if (!_resolve_subject_value(hir_args[arg_index], smt, &di)) continue;
                TRACE_add_note(
                    &trace, trace_id, &ctx->curr_location,
                    "Value '%ld' has the '%s' type! Consider the 'as %s' command!", di.const_value, received, expected
                );
            }
            else {
                variable_info_t vi;
                if (!VRTB_get_info_id(hir_args[arg_index]->storage.var.v_id, &vi, &smt->v)) continue;
                TRACE_add_note(
                    &trace, trace_id, &ctx->curr_location,
                    "Variable '%s' has the '%s' type! Consider the 'as %s' command!", vi.name->body, received, expected
                );

                file_position_t loc;
                _sparce_find_variable_define_location(b, hir_args[arg_index]->storage.var.v_id, &loc);
                TRACE_add_note(&trace, trace_id, &loc, "Variable '%s' declared here!", vi.name->body);
            }

        }

        arg_index++;
    }

    mm_free(hir_args);
    TRACE_print_and_free_trace(&trace);
    return 1;
}

int HIRWLKR_visit_syscall_instruction(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    if (b->op != HIR_SYSC && b->op != HIR_STORE_SYSC) return 1;
    func_info_t fi;
    if (!FNTB_get_info_id(bb->pfunc->f_id, &fi, &smt->f)) {
        return 1;
    }

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
    if (!flatten_input) goto _force_exit_syscall_checker;
    switch (CONF_get_system_type()) {
        case MACHO64: {
            table_size = SYSCHECK_get_macoh_x86_64_syscall_table(&table);
            di.const_value -= 0x2000000;
            break;
        }
        case LINUX64: table_size = SYSCHECK_get_linux_x86_64_syscall_table(&table); break;
        default: goto _force_exit_syscall_checker;
    }

    if (di.const_value >= table_size || di.const_value < 0) {
        trace_id_t trace_id = TRACE_create_root(
            &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
            "Selected architecture doesn't have a syscall for %li value!", di.const_value
        );
        if (di.defined_value == 2) {
            file_position_t loc;
            _sparce_find_variable_define_location(b, number->storage.var.v_id, &loc);
            TRACE_add_note(
                &trace, trace_id, &loc, "Variable '%s' is assigned with this value here",
                _resolve_variable_name(number->storage.var.v_id, smt)
            );
        }
        goto _force_exit_syscall_checker;
    }

    syscall_t syscall = table[di.const_value];
    if (syscall.security > ctx->acceptable_level) {
        TRACE_create_root(
            &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
            "Syscall %i (%s, %s) has security level %i and is dangerous for this acceptance level (%i). Consider deleting this call or reducing the acceptance level.",
            di.const_value, syscall.name, syscall.description, syscall.security, ctx->acceptable_level
        );
        goto _force_exit_syscall_checker;
    }

    trace_id_t wrong_args_trace_id = TRACE_NO_ID;
    for (int arg_index = 1; arg_index < syscall.argc && arg_index < list_size(&b->targ->storage.list.h); arg_index++) {
        int sarg_index = arg_index - 1;
        if (
            HIR_get_tmp_type(flatten_input[arg_index]->t) != syscall.types[sarg_index].t ||
            flatten_input[arg_index]->ptr != syscall.types[sarg_index].ptr
        ) {
            char received[64] = { 0 }, expected[64] = { 0 };
            _create_type_name(syscall.types[sarg_index].t, syscall.types[sarg_index].ptr, expected, sizeof(expected));
            _create_type_name(flatten_input[arg_index]->t, flatten_input[arg_index]->ptr, received, sizeof(received));
            if (wrong_args_trace_id == TRACE_NO_ID) {
                wrong_args_trace_id = TRACE_create_root(
                    &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
                    "Syscall (%s, %s) with number %i has some wrong typed arguments! It can lead to UB, consider to cast them:",
                    syscall.name, syscall.description, di.const_value
                );
            }
            TRACE_add_note(
                &trace, wrong_args_trace_id, &ctx->curr_location,
                "%i argument (%s, %s) should have the '%s' type, but the '%s' is provided! Consider to cast it with 'as %s'.",
                arg_index + 1, syscall.types[sarg_index].name, syscall.types[sarg_index].description, expected, received, expected
            );

            if (!HIR_is_defined_type(flatten_input[arg_index]->t)) {
                file_position_t loc;
                _sparce_find_variable_define_location(b, flatten_input[arg_index]->storage.var.v_id, &loc);
                TRACE_add_note(&trace, wrong_args_trace_id, &loc, "The variable is defined here!");
            }

        }

        if (syscall.types[sarg_index].dereference) {
            HIR_SEM_check_subject_value_and_provide_trace_ex(
                b, bb, flatten_input[arg_index], smt, ctx, 0, "NULL", HIR_VALUE_TRACE_POSSIBLE, "NULL-dereference error"
            );
        }
    }

_force_exit_syscall_checker: {}
    if (flatten_input) mm_free(flatten_input);
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

    if (fi.rtype && fi.rtype->t->t_type != I0_TYPE_TOKEN) {
        trace_t trace;
        TRACE_init_trace(&trace);

        char rtype[64];
        _create_type_name(HIR_get_tmptype_tkn(fi.rtype->t, 0), fi.rtype->t->flags.ptr, rtype, sizeof(rtype));
        TRACE_create_root(
            &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
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

    if (b->op == HIR_SYSC || b->op == HIR_STORE_SYSC) return 1;
    func_info_t fi;
    if (
        b->sarg->t != HIR_FNAME ||
        !FNTB_get_info_id(b->sarg->storage.str.s_id, &fi, &smt->f)
    ) return 1;

    if (
        !fi.rtype ||
        (fi.rtype->t->t_type == I0_TYPE_TOKEN && !fi.rtype->t->flags.ptr)
    ) {
        trace_t trace;
        TRACE_init_trace(&trace);
        TRACE_create_root(
            &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
            "Function '%s' doesn't return any value, but it used as a value. Consider to change the return type.",
            fi.name->body
        );

        TRACE_print_and_free_trace(&trace);
        return 0;
    }

    return 1;
}

int HIRWLKR_ref_to_expression(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;

    trace_t trace;
    TRACE_init_trace(&trace);

    if (HIR_is_tmptype(b->sarg->t)) {
        TRACE_create_root(
            &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location,
            "The danger reference to a temp value! Consider to reference from a variable with this value."
        );
    }

    TRACE_print_and_free_trace(&trace);
    return 1;
}

int HIRWLKR_division_by_zero(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    if (b->op != HIR_iDIV && b->op != HIR_iMOD) return 1;
    const char* zero_error = b->op == HIR_iDIV ? "Division by zero error!" : "Modulo by zero error!";
    if (!HIR_SEM_check_subject_value_and_provide_trace_ex(
            b, bb, b->targ, smt, ctx, 0, NULL, HIR_VALUE_TRACE_EXACT, zero_error
    )) {
        return 0;
    }

    if (b->op == HIR_iDIV) {
        HIR_SEM_check_subject_value_and_provide_trace_ex(
            b, bb, b->sarg, smt, ctx, 0, NULL, HIR_VALUE_TRACE_EXACT,
            "Division of zero! This expression will return 0."
        );
    }

    if (
        !_subject_is_statically_known_not_value(b->targ, smt, 0) &&
        Z3_check_subject_eq_llong_at_block(ctx->z3, bb->pfunc, bb, b->targ, 0) == Z3A_MAYBE
    ) {
        trace_t trace;
        TRACE_init_trace(&trace);
        TRACE_create_root(
            &trace, TRACE_SEVERITY_WARNING, &ctx->curr_location, "Possible %s by zero here! %s can be zero!",
            b->op == HIR_iDIV ? "division" : "modulo",
            HIR_is_vartype(b->targ->t) ? _resolve_variable_name(b->targ->storage.var.v_id, smt) : "Value"
        );
        TRACE_print_and_free_trace(&trace);
    }

    return 1;
}

int HIRWLKR_division_by_one(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    if (b->op != HIR_iDIV && b->op != HIR_iMOD) return 1;
    if (!HIR_SEM_check_subject_value_and_provide_trace_ex(
        b, bb, b->targ, smt, ctx, 1, NULL, HIR_VALUE_TRACE_EXACT,
        "Division by one! This expression won't change anything!"
    )) return 0;
    return 1;
}

static int _resolve_array_source(hir_subject_t* subject, sym_table_t* smt, symbol_id_t* array_id, array_info_t* ai) {
    if (!subject || !HIR_is_vartype(subject->t)) return 0;

    symbol_id_t source_id = subject->storage.var.v_id;
    if (ARTB_get_info(source_id, ai, &smt->a)) {
        if (array_id) *array_id = source_id;
        return 1;
    }

    set_t slaves;
    int found = 0;
    ALLIAS_get_slaves(source_id, &slaves, &smt->m);
    set_foreach (symbol_id_t slave, &slaves) {
        if (ARTB_get_info(slave, ai, &smt->a)) {
            if (array_id) *array_id = slave;
            found = 1;
            break;
        }
    }

    set_free(&slaves);
    return found;
}

static int _subject_can_be_ge_llong_at_block(
    z3_analyzer_t* z3, cfg_func_t* function, cfg_block_t* block, hir_subject_t* subject, sym_table_t* smt, long long value
) {
    defined_variable_t di;
    if (_resolve_subject_value(subject, smt, &di)) {
        if (di.defined_value == 1 || di.defined_value == 2) {
            return di.const_value >= value;
        }
    }

    int z3_answer = Z3_check_subject_ge_llong_at_block(z3, function, block, subject, value);
    return z3_answer == Z3A_YES || z3_answer == Z3A_MAYBE;
}

int HIRWLKR_bad_buffer_move(HIR_VISITOR_ARGS) {
    HIR_VISITOR_ARGS_USE;
    if (
        b->op != HIR_iADD || 
        !b->sarg || !b->targ || !HIR_is_vartype(b->sarg->t)
    ) return 1;

    symbol_id_t buffer_id = NO_SYMBOL_ID;
    array_info_t ai;
    if (!_resolve_array_source(b->sarg, smt, &buffer_id, &ai)) return 1;

    token_t elem_token = { .t_type = ai.elements_info.el_type, .flags = ai.elements_info.el_flags };
    long long buffer_size = ai.size * TKN_convert_type_size(TKN_variable_bitness(&elem_token, 1));
    if (
        _subject_can_be_ge_llong_at_block(ctx->z3, bb->pfunc, bb, b->targ, smt, buffer_size) ||
        Z3_check_subject_lt_llong_at_block(ctx->z3, bb->pfunc, bb, b->targ, 0)
    ) {
        char move_buffer[32] = { 0 };
        const char* move_repr = HIR_is_vartype(b->targ->t) ? _resolve_variable_name(b->targ->storage.var.v_id, smt) : "Value";
        defined_variable_t di;
        if (
            _resolve_subject_value(b->targ, smt, &di) &&
            (di.defined_value == 1 || di.defined_value == 2)
        ) move_repr = _value_name_or_numeric(di.const_value, NULL, move_buffer, sizeof(move_buffer));

        trace_t trace;
        TRACE_init_trace(&trace);

        trace_id_t base = TRACE_create_root(
            &trace, TRACE_SEVERITY_ERROR, &ctx->curr_location, 
            "Possible buffer overflow! Current buffer '%s' is moved by %s that can be not in [0, %lli)",
            _resolve_variable_name(buffer_id, smt), move_repr, buffer_size
        );

        if (HIR_is_vartype(b->targ->t)) {
            file_position_t loc;
            str_memcpy(&loc, &ctx->curr_location, sizeof(file_position_t));
            _sparce_find_variable_define_location(b, b->targ->storage.var.v_id, &loc);
            TRACE_add_note(
                &trace, base, &loc, "Variable '%s' declared here!",
                _resolve_variable_name(b->targ->storage.var.v_id, smt)
            );
        }

        TRACE_print_and_free_trace(&trace);        
    }

    return 1;
}

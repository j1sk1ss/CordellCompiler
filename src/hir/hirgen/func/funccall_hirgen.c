#include <hir/hirgens/hirgens.h>

/*
Check whether a generated HIR argument has the AST argument shape.
Compares pointer depth and scalar conversion class.
Params:
    - `arg` - AST argument node.
    - `hir_arg` - Generated HIR argument subject.

Returns 0 if shapes match, otherwise -1.
*/
static inline int _fit_arg_shape(ast_node_t* arg, hir_subject_t* hir_arg) {
    if (
        !arg || !arg->t || !hir_arg ||
        (arg->t->flags.ptr != hir_arg->ptr)
    ) return -1;
    if (
        HIR_get_convop(HIR_get_tmp_type(hir_arg->t)) !=                 /* Provided */
        HIR_get_convop(HIR_get_tmptype_tkn(arg->t, !arg->t->flags.ptr)) /* Expected */
    ) return -1;
    return 0;
}

/*
Score a custom type argument match for overload resolution.
Params:
    - `arg` - AST argument node.
    - `hir_arg` - Generated HIR argument subject.
    - `smt` - Symtable.

Returns 1 for an exact custom type match, -1 for mismatch, or 0 if not applicable.
*/
static inline int _fit_custom_type(ast_node_t* arg, hir_subject_t* hir_arg, sym_table_t* smt) {
    if (
        !arg || !arg->t || !hir_arg || !HIR_is_vartype(hir_arg->t) ||
        (
            arg->t->t_type != CUSTOM_TYPE_TOKEN && 
            arg->t->t_type != CUSTOM_VARIABLE_TOKEN
        )
    ) return 0;
    variable_info_t vi;
    if (!VRTB_get_info_id(hir_arg->storage.var.v_id, &vi, &smt->v)) return 0;
    if (arg->sinfo.t_id == NO_SYMBOL_ID || vi.v_id == NO_SYMBOL_ID) return -1;
    return arg->sinfo.t_id == vi.v_id ? 
            1 : /* If types are equal */
            -1; /* otherwise - punish */
}

/*
De-overload for functions in HIR.
The idea to determine which function is beign called:
```cpl
function foo(i32 a) -> i0;  : id=0 :
function foo(i32 a) -> i32; : id=1 :
function foo(i8 a) -> 0;    : id=2 :

{
    foo(10 as i32);         : id=0 :
    i32 a = foo(10 as i32); : id=1 :
    foo(1 as i8);           : id=2 :
}
```

Note: De-overload works only with determined arguments. To determine arguments,
the user must use the 'as' keyword for convertion.

The function will change the call function id based on the provided arguemnts and the return type.
Params:
    - `cctx` - CFG context.
    - `smt` - Symtable.

Returns 1 if succeeds.
*/
static symbol_id_t _resolve_function_overload(
    hir_subject_t* callee, symbol_id_t s_id, hir_subject_t* args, sym_table_t* smt, int ret, token_t* out
) {
    out->t_type = I64_TYPE_TOKEN;
    if (!callee) return NO_SYMBOL_ID;

    func_info_t fi;
    if (
        callee->t != HIR_FNAME || 
        !FNTB_get_info_id(callee->storage.str.s_id, &fi, &smt->f)
    ) return callee->storage.str.s_id;

    if (fi.rtype) {
        str_memcpy(out, fi.rtype->t, sizeof(token_t));
    }

    list_t funcs;
    list_init(&funcs);
    if (
        FNTB_collect_info(fi.name, s_id, &funcs, &smt->f) && 
        list_size(&funcs) > 1
    ) {
        int most_fit = -999;
        func_info_t* resolved = NULL;
        int arg_count = list_size(&args->storage.list.h);
        
        foreach (func_info_t* func, &funcs) {
            int fargs = 0;
            fn_iterate_args (func) {
                fargs++;
            }

            int arg_index = 0, fits = 0; 
            if (arg_count == fargs) fits++;
            void** fl_args = list_flatten(&args->storage.list.h);
            if (fl_args) {
                fn_iterate_args (func) {
                    if (arg_count <= arg_index || arg->t->t_type == VAR_ARGUMENTS_TOKEN) break;
                    hir_subject_t* hir_arg = (hir_subject_t*)fl_args[arg_index++];
                    if (!hir_arg) continue;
                    fits += _fit_arg_shape(arg, hir_arg);
                    fits += _fit_custom_type(arg, hir_arg, smt);
                }

                mm_free(fl_args);
            }

            if (fits > most_fit) {
                resolved = func;
                most_fit = fits;
            }
        }

        if (resolved) {
            if (ret && resolved->rtype) str_memcpy(out, resolved->rtype->t, sizeof(token_t));
            callee->storage.str.s_id = resolved->id;
        }
    }

    list_free(&funcs);
    return callee->storage.str.s_id;
}

hir_subject_t* HIR_generate_funccall(ast_node_t* node, hir_ctx_t* ctx, sym_table_t* smt, int ret) {
    HIR_SET_CURRENT_POS(ctx, node);
    hir_subject_t* call_subj = NULL;
    hir_operation_t st_op    = HIR_STORE_UFCLL, op = HIR_UFCLL;
    if (!node || !node->c || !node->c->siblings.n) {
        HIRGEN_ERROR(ctx, "Function call: malformed AST node!");
        return NULL;
    }

    ast_node_t* args_node    = node->c->siblings.n->c;
    
    func_info_t fi = { 0 };
    if (
        node->c->t->t_type != FUNC_NAME_TOKEN || 
        !FNTB_get_info_id(node->c->sinfo.v_id, &fi, &smt->f)
    ) call_subj = HIR_generate_elem(node->c, ctx, smt);
    else {
        op        = fi.flags.external ? HIR_ECLL : HIR_FCLL;
        st_op     = fi.flags.external ? HIR_STORE_ECLL : HIR_STORE_FCLL;
        call_subj = HIR_SUBJ_FUNCNAME(node->c);
        if (node->c->sinfo.s_id != NO_SYMBOL_ID) fi.s_id = node->c->sinfo.s_id;
    }
    
    if (!call_subj) {
        HIRGEN_ERROR(ctx, "Function call: callee generation error!");
        return NULL;
    }

    hir_subject_t* args = HIR_SUBJ_LIST();
    for (ast_node_t* arg = args_node; arg; arg = arg->siblings.n) {
        hir_subject_t* el = HIR_generate_elem(arg, ctx, smt);
        if (!el) {
            HIRGEN_ERROR(ctx, "Function call: argument generation error!");
            HIR_unload_subject(args);
            HIR_unload_subject(call_subj);
            return NULL;
        }

        if (!HIR_is_defined_type(el->t)) {
            HIR_BLOCK1(ctx, HIR_VRUSE, el);
            el = HIR_copy_subject(el);
        }
        
        list_add(&args->storage.list.h, el);
    }
    
    token_t tmp = { 0 };
    func_info_t resolved;
    if (FNTB_get_info_id(_resolve_function_overload(call_subj, fi.s_id, args, smt, ret, &tmp), &resolved, &smt->f)) {
        int arg_offset = 0, arg_count = list_size(&args->storage.list.h);
        fn_iterate_args (&resolved) {
            if (arg_offset++ < arg_count || !arg->c || !arg->c->siblings.n) continue;
            hir_subject_t* el = HIR_generate_elem(arg->c->siblings.n, ctx, smt);
            if (!HIR_is_defined_type(el->t)) {
                HIR_BLOCK1(ctx, HIR_VRUSE, el);
                el = HIR_copy_subject(el);
            }
            
            list_add(&args->storage.list.h, el);
        }
    }

    if (!ret) {
        HIR_BLOCK3(ctx, op, NULL, call_subj, args);
        return NULL;
    }

    hir_subject_t* res = HIR_SUBJ_TMPVAR(
        HIR_get_tmptype_tkn(&tmp, 0), VRTB_add_info(NULL, tmp.t_type, NO_SYMBOL_ID, &tmp.flags, &smt->v)
    );
    
    res->ptr = fi.rtype ? fi.rtype->t->flags.ptr : 0;
    HIR_BLOCK3(ctx, st_op, res, call_subj, args);
    return res;
}

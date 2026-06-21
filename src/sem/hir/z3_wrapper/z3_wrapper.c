#include <sem/hir/z3_wrapper.h>

#ifdef CPL_ENABLE_Z3
typedef struct {
    Z3_context  ctx;
    Z3_solver   solver;
    cfg_func_t* function;
    map_t       vars;
    map_t       labels;
    int         complete;
} z3_func_ctx_t;

static int _z3c_bv_bits(hir_subject_type_t t) {
    switch (t) {
        case HIR_TMPVARI0:    case HIR_STKVARI0: case HIR_GLBVARI0: return 1;
        case HIR_TMPVARI8:    case HIR_TMPVARU8:
        case HIR_STKVARI8:    case HIR_STKVARU8:
        case HIR_GLBVARI8:    case HIR_GLBVARU8:
        case HIR_I8NUMBER:    case HIR_U8NUMBER:
        case HIR_I8CONSTVAL:  case HIR_U8CONSTVAL:  return 8;
        case HIR_TMPVARI16:   case HIR_TMPVARU16:
        case HIR_STKVARI16:   case HIR_STKVARU16:
        case HIR_GLBVARI16:   case HIR_GLBVARU16:
        case HIR_I16NUMBER:   case HIR_U16NUMBER:
        case HIR_I16CONSTVAL: case HIR_U16CONSTVAL: return 16;
        case HIR_TMPVARI32:   case HIR_TMPVARU32:
        case HIR_STKVARI32:   case HIR_STKVARU32:
        case HIR_GLBVARI32:   case HIR_GLBVARU32:
        case HIR_I32NUMBER:   case HIR_U32NUMBER:
        case HIR_I32CONSTVAL: case HIR_U32CONSTVAL: return 32;
        default: return 64;
    }
}

static inline int _z3c_is_bool_subject(hir_subject_t* s) {
    return s && !s->ptr && (
        s->t == HIR_TMPVARI0 ||
        s->t == HIR_STKVARI0 ||
        s->t == HIR_GLBVARI0
    );
}

static inline int _z3c_is_float_subject(hir_subject_t* s) {
    return s && !s->ptr && HIR_is_float(s->t);
}

static Z3_sort _z3c_sort_for_subject(z3_func_ctx_t* fctx, hir_subject_t* s) {
    Z3_context ctx = fctx->ctx;
    if (!s || s->ptr) return Z3_mk_bv_sort(ctx, 64);
    if (_z3c_is_bool_subject(s))  return Z3_mk_bool_sort(ctx);
    if (_z3c_is_float_subject(s)) return Z3_mk_real_sort(ctx);
    if (
        HIR_is_vartype(s->t) || 
        HIR_is_defined_type(s->t)
    ) return Z3_mk_bv_sort(ctx, _z3c_bv_bits(s->t));
    return Z3_mk_uninterpreted_sort(ctx, Z3_mk_string_symbol(ctx, "HIR_opaque"));
}

static const char* _z3c_ty_prefix(hir_subject_type_t t) {
    switch (t) {
        case HIR_TMPVARI0:  case HIR_STKVARI0:  case HIR_GLBVARI0:  return "i0";
        case HIR_TMPVARI8:  case HIR_STKVARI8:  case HIR_GLBVARI8:  return "i8";
        case HIR_TMPVARU8:  case HIR_STKVARU8:  case HIR_GLBVARU8:  return "u8";
        case HIR_TMPVARI16: case HIR_STKVARI16: case HIR_GLBVARI16: return "i16";
        case HIR_TMPVARU16: case HIR_STKVARU16: case HIR_GLBVARU16: return "u16";
        case HIR_TMPVARI32: case HIR_STKVARI32: case HIR_GLBVARI32: return "i32";
        case HIR_TMPVARU32: case HIR_STKVARU32: case HIR_GLBVARU32: return "u32";
        case HIR_TMPVARI64: case HIR_STKVARI64: case HIR_GLBVARI64: return "i64";
        case HIR_TMPVARU64: case HIR_STKVARU64: case HIR_GLBVARU64: return "u64";
        case HIR_TMPVARF32: case HIR_STKVARF32: case HIR_GLBVARF32: return "f32";
        case HIR_TMPVARF64: case HIR_STKVARF64: case HIR_GLBVARF64: return "f64";
        case HIR_TMPVARARR: case HIR_STKVARARR: case HIR_GLBVARARR: return "arr";
        default: return "v";
    }
}

static const char* _z3c_storage_prefix(hir_subject_type_t t) {
    switch (t) {
        case HIR_TMPVARARR: case HIR_TMPVARF64: case HIR_TMPVARU64:
        case HIR_TMPVARI64: case HIR_TMPVARF32: case HIR_TMPVARU32:
        case HIR_TMPVARI32: case HIR_TMPVARU16: case HIR_TMPVARI16:
        case HIR_TMPVARU8:  case HIR_TMPVARI8:  case HIR_TMPVARI0: return "tmp";
        case HIR_GLBVARARR: case HIR_GLBVARF64: case HIR_GLBVARU64:
        case HIR_GLBVARI64: case HIR_GLBVARF32: case HIR_GLBVARU32:
        case HIR_GLBVARI32: case HIR_GLBVARU16: case HIR_GLBVARI16:
        case HIR_GLBVARU8:  case HIR_GLBVARI8:  case HIR_GLBVARI0: return "global";
        default: return "stack";
    }
}

static Z3_ast _z3c_var(z3_func_ctx_t* fctx, hir_subject_t* s) {
    if (!s || !HIR_is_vartype(s->t)) return NULL;

    Z3_ast cached = NULL;
    if (map_get(&fctx->vars, (long)s->storage.var.v_id, (void**)&cached)) {
        return cached;
    }

    char name[128] = { 0 };
    if (s->ptr) {
        snprintf(
            name,
            sizeof(name),
            "%s_%s_p%i_%lld",
            _z3c_ty_prefix(s->t),
            _z3c_storage_prefix(s->t),
            s->ptr,
            (long long)s->storage.var.v_id
        );
    } 
    else {
        snprintf(
            name,
            sizeof(name),
            "%s_%s_%lld",
            _z3c_ty_prefix(s->t),
            _z3c_storage_prefix(s->t),
            (long long)s->storage.var.v_id
        );
    }

    Z3_ast ast = Z3_mk_const(fctx->ctx, Z3_mk_string_symbol(fctx->ctx, name), _z3c_sort_for_subject(fctx, s));
    Z3_inc_ref(fctx->ctx, ast);
    map_put(&fctx->vars, (long)s->storage.var.v_id, ast);
    return ast;
}

static long long _z3c_subject_int_value(hir_subject_t* s) {
    if (!s) return 0;
    if (HIR_is_defined_type(s->t) == 2) return s->storage.cnst.value;
    if (HIR_is_defined_type(s->t) == 1 && s->storage.num.value) {
        return s->storage.num.value->to_llong(s->storage.num.value);
    }

    return 0;
}

static Z3_ast _z3c_literal(z3_func_ctx_t* fctx, hir_subject_t* s) {
    Z3_sort sort = _z3c_sort_for_subject(fctx, s);
    if (_z3c_is_bool_subject(s)) {
        return _z3c_subject_int_value(s)
            ? Z3_mk_true(fctx->ctx)
            : Z3_mk_false(fctx->ctx);
    }

    if (_z3c_is_float_subject(s) && HIR_is_defined_type(s->t) == 1 && s->storage.num.value) {
        return Z3_mk_numeral(fctx->ctx, s->storage.num.value->body, sort);
    }

    return Z3_mk_int64(fctx->ctx, _z3c_subject_int_value(s), sort);
}

static Z3_ast _z3c_expr(z3_func_ctx_t* fctx, hir_subject_t* s) {
    if (!s) return NULL;
    if (HIR_is_vartype(s->t))      return _z3c_var(fctx, s);
    if (HIR_is_defined_type(s->t)) return _z3c_literal(fctx, s);
    return NULL;
}

static Z3_ast _z3c_zero_for_sort(Z3_context ctx, Z3_sort sort) {
    switch (Z3_get_sort_kind(ctx, sort)) {
        case Z3_BOOL_SORT: return Z3_mk_false(ctx);
        case Z3_BV_SORT:   return Z3_mk_int(ctx, 0, sort);
        case Z3_INT_SORT:
        case Z3_REAL_SORT: return Z3_mk_numeral(ctx, "0", sort);
        default: return NULL;
    }
}

static Z3_ast _z3c_as_bool(z3_func_ctx_t* fctx, Z3_ast value) {
    if (!value) return NULL;
    Z3_context ctx = fctx->ctx;
    Z3_sort sort = Z3_get_sort(ctx, value);
    switch (Z3_get_sort_kind(ctx, sort)) {
        case Z3_BOOL_SORT: return value;
        case Z3_BV_SORT:
        case Z3_INT_SORT:
        case Z3_REAL_SORT: {
            Z3_ast zero = _z3c_zero_for_sort(ctx, sort);
            return zero ? Z3_mk_distinct(ctx, 2, (Z3_ast[]){ value, zero }) : NULL;
        }
        default: return NULL;
    }
}

static Z3_ast _z3c_coerce(z3_func_ctx_t* fctx, Z3_ast value, Z3_sort target, int is_signed) {
    if (!value) return NULL;
    Z3_context ctx = fctx->ctx;
    Z3_sort source = Z3_get_sort(ctx, value);
    if (Z3_is_eq_sort(ctx, source, target)) return value;

    Z3_sort_kind source_kind = Z3_get_sort_kind(ctx, source);
    Z3_sort_kind target_kind = Z3_get_sort_kind(ctx, target);

    if (source_kind == Z3_BOOL_SORT && target_kind == Z3_BV_SORT) {
        return Z3_mk_ite(ctx, value, Z3_mk_int(ctx, 1, target), Z3_mk_int(ctx, 0, target));
    }

    if (source_kind == Z3_BV_SORT && target_kind == Z3_BOOL_SORT) {
        return _z3c_as_bool(fctx, value);
    }

    if (source_kind == Z3_BV_SORT && target_kind == Z3_BV_SORT) {
        unsigned source_bits = Z3_get_bv_sort_size(ctx, source);
        unsigned target_bits = Z3_get_bv_sort_size(ctx, target);
        if (source_bits < target_bits) {
            unsigned extra = target_bits - source_bits;
            return is_signed ? Z3_mk_sign_ext(ctx, extra, value) : Z3_mk_zero_ext(ctx, extra, value);
        }

        if (source_bits > target_bits) {
            return Z3_mk_extract(ctx, target_bits - 1, 0, value);
        }
    }

    return NULL;
}

static Z3_ast _z3c_binary(z3_func_ctx_t* fctx, hir_block_t* h) {
    Z3_ast lhs = _z3c_expr(fctx, h->sarg);
    Z3_ast rhs = _z3c_expr(fctx, h->targ);
    if (!lhs || !rhs) return NULL;

    Z3_context ctx   = fctx->ctx;
    Z3_sort lhs_sort = Z3_get_sort(ctx, lhs);
    Z3_sort rhs_sort = Z3_get_sort(ctx, rhs);
    if (!Z3_is_eq_sort(ctx, lhs_sort, rhs_sort)) {
        Z3_sort_kind lhs_kind = Z3_get_sort_kind(ctx, lhs_sort);
        Z3_sort_kind rhs_kind = Z3_get_sort_kind(ctx, rhs_sort);
        if (
            lhs_kind != Z3_BV_SORT || 
            rhs_kind != Z3_BV_SORT
        ) rhs = _z3c_coerce(fctx, rhs, lhs_sort, HIR_is_sign(h->targ ? h->targ->t : HIR_NOTYPE));
        else {
            unsigned lhs_bits = Z3_get_bv_sort_size(ctx, lhs_sort);
            unsigned rhs_bits = Z3_get_bv_sort_size(ctx, rhs_sort);
            Z3_sort target_sort = lhs_bits >= rhs_bits ? lhs_sort : rhs_sort;
            lhs = _z3c_coerce(fctx, lhs, target_sort, HIR_is_sign(h->sarg ? h->sarg->t : HIR_NOTYPE));
            rhs = _z3c_coerce(fctx, rhs, target_sort, HIR_is_sign(h->targ ? h->targ->t : HIR_NOTYPE));
        }
        
        if (!lhs || !rhs) return NULL;
    }

    lhs_sort = Z3_get_sort(ctx, lhs);

    Z3_sort_kind kind = Z3_get_sort_kind(ctx, lhs_sort);
    int signed_op = HIR_is_sign(h->sarg ? h->sarg->t : HIR_NOTYPE);

    if (h->op == HIR_iAND || h->op == HIR_iOR) {
        Z3_ast args[] = { _z3c_as_bool(fctx, lhs), _z3c_as_bool(fctx, rhs) };
        if (!args[0] || !args[1]) return NULL;
        return h->op == HIR_iAND ? Z3_mk_and(ctx, 2, args) : Z3_mk_or(ctx, 2, args);
    }

    if (kind == Z3_BV_SORT) {
        switch (h->op) {
            case HIR_iADD:  return Z3_mk_bvadd(ctx, lhs, rhs);
            case HIR_iSUB:  return Z3_mk_bvsub(ctx, lhs, rhs);
            case HIR_iMUL:  return Z3_mk_bvmul(ctx, lhs, rhs);
            case HIR_iDIV:  return signed_op ? Z3_mk_bvsdiv(ctx, lhs, rhs) : Z3_mk_bvudiv(ctx, lhs, rhs);
            case HIR_iMOD:  return signed_op ? Z3_mk_bvsrem(ctx, lhs, rhs) : Z3_mk_bvurem(ctx, lhs, rhs);
            case HIR_iLRG:  return signed_op ? Z3_mk_bvsgt(ctx, lhs, rhs) : Z3_mk_bvugt(ctx, lhs, rhs);
            case HIR_iLGE:  return signed_op ? Z3_mk_bvsge(ctx, lhs, rhs) : Z3_mk_bvuge(ctx, lhs, rhs);
            case HIR_iLWR:  return signed_op ? Z3_mk_bvslt(ctx, lhs, rhs) : Z3_mk_bvult(ctx, lhs, rhs);
            case HIR_iLRE:  return signed_op ? Z3_mk_bvsle(ctx, lhs, rhs) : Z3_mk_bvule(ctx, lhs, rhs);
            case HIR_iCMP:  return Z3_mk_eq(ctx, lhs, rhs);
            case HIR_iNMP:  return Z3_mk_not(ctx, Z3_mk_eq(ctx, lhs, rhs));
            case HIR_iBLFT: return Z3_mk_bvshl(ctx, lhs, rhs);
            case HIR_iBRHT: return signed_op ? Z3_mk_bvashr(ctx, lhs, rhs) : Z3_mk_bvlshr(ctx, lhs, rhs);
            case HIR_bAND:  return Z3_mk_bvand(ctx, lhs, rhs);
            case HIR_bOR:   return Z3_mk_bvor(ctx, lhs, rhs);
            case HIR_bXOR:  return Z3_mk_bvxor(ctx, lhs, rhs);
            default: break;
        }
    } 
    else if (kind == Z3_REAL_SORT || kind == Z3_INT_SORT) {
        Z3_ast args[] = { lhs, rhs };
        switch (h->op) {
            case HIR_iADD: return Z3_mk_add(ctx, 2, args);
            case HIR_iSUB: return Z3_mk_sub(ctx, 2, args);
            case HIR_iMUL: return Z3_mk_mul(ctx, 2, args);
            case HIR_iDIV: return Z3_mk_div(ctx, lhs, rhs);
            case HIR_iLRG: return Z3_mk_gt(ctx, lhs, rhs);
            case HIR_iLGE: return Z3_mk_ge(ctx, lhs, rhs);
            case HIR_iLWR: return Z3_mk_lt(ctx, lhs, rhs);
            case HIR_iLRE: return Z3_mk_le(ctx, lhs, rhs);
            case HIR_iCMP: return Z3_mk_eq(ctx, lhs, rhs);
            case HIR_iNMP: return Z3_mk_not(ctx, Z3_mk_eq(ctx, lhs, rhs));
            default: break;
        }
    }

    return NULL;
}

static int _z3c_assert_assign(z3_func_ctx_t* fctx, hir_subject_t* dst_subject, Z3_ast value) {
    Z3_ast dst = _z3c_var(fctx, dst_subject);
    if (!dst || !value) return 0;
    value = _z3c_coerce(fctx, value, Z3_get_sort(fctx->ctx, dst), HIR_is_sign(dst_subject->t));
    if (!value) return 0;
    Z3_solver_assert(fctx->ctx, fctx->solver, Z3_mk_eq(fctx->ctx, dst, value));
    return 1;
}

static int _z3c_lower_instruction(z3_func_ctx_t* fctx, hir_block_t* h) {
    if (!h || h->unused) return 1;
    switch (h->op) {
        case HIR_MKLB: {
            if (h->farg) map_put(&fctx->labels, (long)h->farg->id, h->farg);
            return 1;
        }
        case HIR_STORE: return _z3c_assert_assign(fctx, h->farg, _z3c_expr(fctx, h->sarg));
        case HIR_NOT: {
            Z3_ast value = _z3c_as_bool(fctx, _z3c_expr(fctx, h->sarg));
            return _z3c_assert_assign(fctx, h->farg, value ? Z3_mk_not(fctx->ctx, value) : NULL);
        }
        case HIR_NEG: {
            Z3_ast value = _z3c_expr(fctx, h->sarg);
            if (!value) return 0;
            Z3_sort sort = Z3_get_sort(fctx->ctx, value);
            if (Z3_get_sort_kind(fctx->ctx, sort) != Z3_BV_SORT) return 0;
            return _z3c_assert_assign(fctx, h->farg, Z3_mk_bvnot(fctx->ctx, value));
        }
        case HIR_TPTR: case HIR_TF64: case HIR_TF32:
        case HIR_TI64: case HIR_TI32: case HIR_TI16: case HIR_TI8:
        case HIR_TU64: case HIR_TU32: 
        case HIR_TU16: case HIR_TU8:  return _z3c_assert_assign(fctx, h->farg, _z3c_expr(fctx, h->sarg));
        case HIR_iADD: case HIR_iSUB: case HIR_iMUL: case HIR_iDIV:
        case HIR_iMOD: case HIR_iLRG: case HIR_iLGE: case HIR_iLWR:
        case HIR_iLRE: case HIR_iCMP: case HIR_iNMP: case HIR_iAND:
        case HIR_iOR: case HIR_iBLFT: case HIR_iBRHT: case HIR_bAND:
        case HIR_bOR: case HIR_bXOR:  return _z3c_assert_assign(fctx, h->farg, _z3c_binary(fctx, h));
        case HIR_FARGLD:
        case HIR_STARGLD:
        case HIR_VARDECL:
        case HIR_ARRDECL:
        case HIR_STRDECL:
        case HIR_REF: case HIR_GDREF:
        case HIR_STORE_UFCLL: case HIR_STORE_FCLL: case HIR_STORE_ECLL: case HIR_STORE_SYSC: {
            if (h->farg && HIR_is_vartype(h->farg->t)) _z3c_var(fctx, h->farg);
            return 1;
        }
        case HIR_IFOP2:
        case HIR_JMP:
        case HIR_FRET:
        case HIR_FDCL:
        case HIR_FEND:
        case HIR_STRT:
        case HIR_STEND:
        case HIR_SETPOS:
        case HIR_VRUSE:
        case HIR_MKSCOPE:
        case HIR_ENDSCOPE:
        case HIR_NOP:     return 1;
        default:          return 0;
    }
}

static z3_func_ctx_t* _z3c_build_function(z3_analyzer_t* analyzer, cfg_func_t* function) {
    z3_func_ctx_t* fctx = (z3_func_ctx_t*)mm_malloc(sizeof(z3_func_ctx_t));
    if (!fctx) return NULL;
    str_memset(fctx, 0, sizeof(z3_func_ctx_t));
    fctx->ctx = analyzer->ctx;
    fctx->function = function;
    fctx->complete = 1;
    map_init(&fctx->vars, MAP_NO_CMP);
    map_init(&fctx->labels, MAP_NO_CMP);
    fctx->solver = Z3_mk_solver(fctx->ctx);
    Z3_solver_inc_ref(fctx->ctx, fctx->solver);

    foreach (cfg_block_t* bb, &function->blocks) {
        iterate_hir_instructions (bb) {
            if (!_z3c_lower_instruction(fctx, hh)) fctx->complete = 0;
            if (
                hh->op == HIR_MKLB && 
                hh->farg
            ) map_put(&fctx->labels, (long)hh->farg->id, bb);
            iterate_hir_args (hir_subject_t* s, hh, 0) {
                if (s && HIR_is_vartype(s->t)) _z3c_var(fctx, s);
            }
        }
    }

    return fctx;
}

static int _z3c_unload_function(z3_func_ctx_t* fctx) {
    if (!fctx) return 1;
    Z3_solver_dec_ref(fctx->ctx, fctx->solver);
    map_free(&fctx->vars);
    map_free(&fctx->labels);
    return mm_free(fctx);
}

z3_analyzer_t* Z3A_create(cfg_ctx_t* cfg, sym_table_t* smt) {
    if (!cfg || !smt) return NULL;
    z3_analyzer_t* analyzer = (z3_analyzer_t*)mm_malloc(sizeof(z3_analyzer_t));
    if (!analyzer) return NULL;
    str_memset(analyzer, 0, sizeof(z3_analyzer_t));
    analyzer->cfg_ctx = cfg;
    analyzer->smt = smt;
    analyzer->cfg = Z3_mk_config();
    analyzer->ctx = Z3_mk_context_rc(analyzer->cfg);
    map_init(&analyzer->funcs, MAP_NO_CMP);
    return analyzer;
}

int Z3A_unload(z3_analyzer_t* analyzer) {
    if (!analyzer) return 1;
    map_foreach (z3_func_ctx_t* fctx, &analyzer->funcs) {
        _z3c_unload_function(fctx);
    }

    map_free(&analyzer->funcs);
    Z3_del_context(analyzer->ctx);
    Z3_del_config(analyzer->cfg);
    return mm_free(analyzer);
}

static z3_func_ctx_t* _z3c_get_function(z3_analyzer_t* analyzer, cfg_func_t* function) {
    if (!analyzer || !function) return NULL;
    z3_func_ctx_t* fctx = NULL;
    if (map_get(&analyzer->funcs, (long)function->f_id, (void**)&fctx)) {
        return fctx;
    }

    fctx = _z3c_build_function(analyzer, function);
    if (!fctx) return NULL;
    map_put(&analyzer->funcs, (long)function->f_id, fctx);
    return fctx;
}

static int _z3c_block_count(cfg_func_t* function) {
    int count = 0;
    foreach (cfg_block_t* bb, &function->blocks) {
        (void)bb;
        count++;
    }

    return count;
}

static Z3_ast _z3c_edge_condition(z3_func_ctx_t* fctx, cfg_block_t* src, cfg_block_t* dst) {
    if (!src || !src->hmap.exit || src->hmap.exit->op != HIR_IFOP2) {
        return Z3_mk_true(fctx->ctx);
    }

    Z3_ast cond = _z3c_as_bool(fctx, _z3c_expr(fctx, src->hmap.exit->farg));
    if (!cond) return NULL;
    if (dst == src->l) return cond;
    if (dst == src->jmp) return Z3_mk_not(fctx->ctx, cond);
    return NULL;
}

static inline int _z3c_ast_is_true(Z3_context ctx, Z3_ast ast) {
    return ast && Z3_get_bool_value(ctx, ast) == Z3_L_TRUE;
}

static int _z3c_check_current(z3_func_ctx_t* fctx, Z3_ast extra) {
    int pushed = 0;
    if (extra && !_z3c_ast_is_true(fctx->ctx, extra)) {
        Z3_solver_push(fctx->ctx, fctx->solver);
        Z3_solver_assert(fctx->ctx, fctx->solver, extra);
        pushed = 1;
    }

    Z3_lbool check = Z3_solver_check(fctx->ctx, fctx->solver);
    if (pushed) Z3_solver_pop(fctx->ctx, fctx->solver, 1);
    return check == Z3_L_TRUE;
}

static int _z3c_any_path_to_block(z3_func_ctx_t* fctx, cfg_block_t* curr, cfg_block_t* target, int depth, int max_depth) {
    if (!curr || depth > max_depth) return 0;
    if (curr == target) return _z3c_check_current(fctx, NULL);

    cfg_block_t* succs[] = { curr->l, curr->jmp };
    for (int i = 0; i < 2; ++i) {
        cfg_block_t* succ = succs[i];
        if (!succ) continue;
        Z3_ast cond = _z3c_edge_condition(fctx, curr, succ);
        if (!cond) return 0;
        Z3_solver_push(fctx->ctx, fctx->solver);
        if (!_z3c_ast_is_true(fctx->ctx, cond)) Z3_solver_assert(fctx->ctx, fctx->solver, cond);
        int found = _z3c_any_path_to_block(fctx, succ, target, depth + 1, max_depth);
        Z3_solver_pop(fctx->ctx, fctx->solver, 1);
        if (found) return 1;
    }

    return 0;
}

static int _z3c_any_terminal_path(z3_func_ctx_t* fctx, cfg_block_t* curr, Z3_ast extra, int depth, int max_depth) {
    if (!curr || depth > max_depth) return 0;
    if (!curr->l && !curr->jmp) return _z3c_check_current(fctx, extra);

    cfg_block_t* succs[] = { curr->l, curr->jmp };
    for (int i = 0; i < 2; ++i) {
        cfg_block_t* succ = succs[i];
        if (!succ) continue;
        Z3_ast cond = _z3c_edge_condition(fctx, curr, succ);
        if (!cond) return 0;
        Z3_solver_push(fctx->ctx, fctx->solver);
        if (!_z3c_ast_is_true(fctx->ctx, cond)) Z3_solver_assert(fctx->ctx, fctx->solver, cond);
        int found = _z3c_any_terminal_path(fctx, succ, extra, depth + 1, max_depth);
        Z3_solver_pop(fctx->ctx, fctx->solver, 1);
        if (found) return 1;
    }

    return 0;
}

static int _z3c_can_reach_label(z3_analyzer_t* analyzer, cfg_func_t* function, long label_id) {
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    if (!fctx || !fctx->complete) return -1;
    cfg_block_t* target = NULL;
    if (!map_get(&fctx->labels, label_id, (void**)&target) || !target) return 0;
    cfg_block_t* entry = (cfg_block_t*)list_get_head(&function->blocks);
    int max_depth = MAX(16, _z3c_block_count(function) * 4);
    return _z3c_any_path_to_block(fctx, entry, target, 0, max_depth);
}

static int _z3c_can_vid_be_equal(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value) {
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    if (!fctx || !fctx->complete) return -1;

    Z3_ast var = NULL;
    if (!map_get(&fctx->vars, (long)v_id, (void**)&var) || !var) return 3;

    Z3_sort sort = Z3_get_sort(fctx->ctx, var);
    Z3_ast val = Z3_get_sort_kind(fctx->ctx, sort) == Z3_BOOL_SORT
        ? (value ? Z3_mk_true(fctx->ctx) : Z3_mk_false(fctx->ctx))
        : Z3_mk_int64(fctx->ctx, value, sort);
    Z3_ast eq = Z3_mk_eq(fctx->ctx, var, val);
    Z3_ast neq = Z3_mk_not(fctx->ctx, eq);

    cfg_block_t* entry = (cfg_block_t*)list_get_head(&function->blocks);
    int max_depth = MAX(16, _z3c_block_count(function) * 4);
    int can_equal = _z3c_any_terminal_path(fctx, entry, eq, 0, max_depth);
    int can_differ = _z3c_any_terminal_path(fctx, entry, neq, 0, max_depth);

    if (can_equal && !can_differ) return 1;
    if (can_equal && can_differ) return 2;
    if (!can_equal && can_differ) return 0;
    return 3;
}

int Z3_can_vid_be_equal_ctx(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value) {
    int result = _z3c_can_vid_be_equal(analyzer, function, v_id, value);
    if (result >= 0) return result;
    return 2;
}

int Z3_can_reach_label_ctx(z3_analyzer_t* analyzer, cfg_func_t* function, long l_id) {
    int result = _z3c_can_reach_label(analyzer, function, l_id);
    if (result >= 0) return result;
    return 1;
}
#else
z3_analyzer_t* Z3A_create(cfg_ctx_t* cfg, sym_table_t* smt) {
    (void)cfg; (void)smt;
    return NULL;
}

int Z3A_unload(z3_analyzer_t* analyzer) {
    (void)analyzer;
    return 1;
}

int Z3_can_vid_be_equal_ctx(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value) {
    (void)analyzer; (void)function; (void)v_id; (void)value;
    return 2;
}

int Z3_can_reach_label_ctx(z3_analyzer_t* analyzer, cfg_func_t* function, long l_id) {
    (void)analyzer; (void)function; (void)l_id;
    return 1;
}
#endif
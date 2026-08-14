#include <sem/hir/z3_wrapper.h>

const char* Z3_result_name(int result) {
    switch (result) {
        case Z3A_NO:      return "no";
        case Z3A_YES:     return "yes";
        case Z3A_MAYBE:   return "maybe";
        case Z3A_UNKNOWN: return "unknown";
        default:          return "invalid";
    }
}

#ifdef CPL_ENABLE_Z3
typedef struct {
    Z3_context  ctx;
    Z3_solver   solver;
    cfg_func_t* function;
    map_t       vars;
    map_t       vars_by_id;
    map_t       labels;
    int         complete;
} z3_func_ctx_t;

static void _z3c_error_handler(Z3_context ctx, Z3_error_code error) {
    fprintf(
        stderr,
        "z3_wrapper: Z3 error %d: %s\n",
        (int)error,
        Z3_get_error_msg(ctx, error)
    );
    fflush(stderr);
}

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

static inline int _z3c_is_supported_bv_width(int bits) {
    return bits == 1 || bits == 8 || bits == 16 || bits == 32 || bits == 64;
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

static inline int _z3c_is_array_subject(hir_subject_t* s) {
    return s && !s->ptr && HIR_is_arrtype(s->t);
}

static inline int _z3c_is_signed_subject(hir_subject_t* s) {
    return s && !s->ptr && HIR_is_sign(s->t);
}

static inline int _z3c_op_is_shift(int op) {
    return op == HIR_iBLFT || op == HIR_iBRHT;
}

static inline int _z3c_op_is_logical(int op) {
    return op == HIR_iAND || op == HIR_iOR;
}

static inline int _z3c_op_is_equality(int op) {
    return op == HIR_iCMP || op == HIR_iNMP;
}

static Z3_sort _z3c_sort_for_subject(z3_func_ctx_t* fctx, hir_subject_t* s) {
    Z3_context ctx = fctx->ctx;
    if (!s || s->ptr) return Z3_mk_bv_sort(ctx, 64);
    if (_z3c_is_bool_subject(s))  return Z3_mk_bool_sort(ctx);
    if (_z3c_is_float_subject(s)) return Z3_mk_real_sort(ctx);
    if (_z3c_is_array_subject(s)) return Z3_mk_uninterpreted_sort(ctx, Z3_mk_string_symbol(ctx, "HIR_array"));
    if (
        HIR_is_vartype(s->t) ||
        HIR_is_defined_type(s->t)
    ) {
        int bits = _z3c_bv_bits(s->t);
        if (!_z3c_is_supported_bv_width(bits)) {
            printf(
                "z3_wrapper: invalid bitvector width %d for subject type %d\n",
                bits,
                (int)s->t
            );
            return Z3_mk_uninterpreted_sort(ctx, Z3_mk_string_symbol(ctx, "HIR_invalid_bv"));
        }

        return Z3_mk_bv_sort(ctx, bits);
    }

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

static long _z3c_var_key(hir_subject_t* s) {
    long key = (long)s->storage.var.v_id;
    key = key * 1315423911L + (long)s->t * 2654435761L;
    key = key * 31L + (long)s->ptr;
    return key;
}

static Z3_ast _z3c_var(z3_func_ctx_t* fctx, hir_subject_t* s) {
    if (!s || !HIR_is_vartype(s->t) || _z3c_is_array_subject(s)) return NULL;

    long key = _z3c_var_key(s);
    Z3_ast cached = NULL;
    if (map_get(&fctx->vars, key, (void**)&cached)) return cached;

    char name[128] = { 0 };
    if (s->ptr) {
        snprintf(
            name, sizeof(name), "%s_%s_p%i_%lld", _z3c_ty_prefix(s->t), _z3c_storage_prefix(s->t), s->ptr, (long long)s->storage.var.v_id
        );
    }
    else {
        snprintf(
            name, sizeof(name), "%s_%s_%lld", _z3c_ty_prefix(s->t), _z3c_storage_prefix(s->t), (long long)s->storage.var.v_id
        );
    }

    Z3_ast ast = Z3_mk_const(fctx->ctx, Z3_mk_string_symbol(fctx->ctx, name), _z3c_sort_for_subject(fctx, s));
    Z3_inc_ref(fctx->ctx, ast);
    map_put(&fctx->vars, key, ast);

    Z3_ast by_id = NULL;
    if (!map_get(&fctx->vars_by_id, (long)s->storage.var.v_id, (void**)&by_id)) {
        map_put(&fctx->vars_by_id, (long)s->storage.var.v_id, ast);
    }

    return ast;
}

static Z3_ast _z3c_var_by_vid(z3_func_ctx_t* fctx, symbol_id_t v_id) {
    Z3_ast var = NULL;
    if (!map_get(&fctx->vars_by_id, (long)v_id, (void**)&var)) return NULL;
    return var;
}

static long long _z3c_subject_int_value(hir_subject_t* s) {
    if (!s) return 0;
    if (HIR_is_defined_type(s->t) == 2) return s->storage.cnst.value;
    if (HIR_is_defined_type(s->t) == 1 && s->storage.num.value) {
        return s->storage.num.value->to_llong(s->storage.num.value);
    }

    return 0;
}

static Z3_ast _z3c_value_for_sort(z3_func_ctx_t* fctx, long long value, Z3_sort sort) {
    Z3_context ctx = fctx->ctx;
    switch (Z3_get_sort_kind(ctx, sort)) {
        case Z3_BOOL_SORT: return value ? Z3_mk_true(ctx) : Z3_mk_false(ctx);
        case Z3_BV_SORT:   return Z3_mk_int64(ctx, value, sort);
        case Z3_INT_SORT:
        case Z3_REAL_SORT: {
            char buf[64] = { 0 };
            snprintf(buf, sizeof(buf), "%lld", value);
            return Z3_mk_numeral(ctx, buf, sort);
        }
        default: return NULL;
    }
}

static Z3_ast _z3c_literal(z3_func_ctx_t* fctx, hir_subject_t* s) {
    Z3_sort sort = _z3c_sort_for_subject(fctx, s);
    if (Z3_get_sort_kind(fctx->ctx, sort) == Z3_UNINTERPRETED_SORT) return NULL;
    if (_z3c_is_bool_subject(s)) {
        return _z3c_subject_int_value(s)
            ? Z3_mk_true(fctx->ctx)
            : Z3_mk_false(fctx->ctx);
    }

    if (_z3c_is_float_subject(s) && HIR_is_defined_type(s->t) == 1 && s->storage.num.value) {
        return Z3_mk_numeral(fctx->ctx, s->storage.num.value->body, sort);
    }

    return _z3c_value_for_sort(fctx, _z3c_subject_int_value(s), sort);
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


static int _z3c_same_sort_raw(Z3_context ctx, Z3_ast lhs, Z3_ast rhs) {
    if (!lhs || !rhs) return 0;
    Z3_sort lhs_sort = Z3_get_sort(ctx, lhs);
    Z3_sort rhs_sort = Z3_get_sort(ctx, rhs);
    Z3_sort_kind lhs_kind = Z3_get_sort_kind(ctx, lhs_sort);
    Z3_sort_kind rhs_kind = Z3_get_sort_kind(ctx, rhs_sort);

    if (lhs_kind != rhs_kind) return 0;

    switch (lhs_kind) {
        case Z3_BV_SORT:
            return Z3_get_bv_sort_size(ctx, lhs_sort) == Z3_get_bv_sort_size(ctx, rhs_sort);
        case Z3_BOOL_SORT:
        case Z3_INT_SORT:
        case Z3_REAL_SORT:
            return 1;
        default:
            return Z3_is_eq_sort(ctx, lhs_sort, rhs_sort);
    }
}

static Z3_ast _z3c_mk_eq_checked_raw(z3_func_ctx_t* fctx, Z3_ast lhs, Z3_ast rhs, const char* where) {
    if (!fctx || !lhs || !rhs) return NULL;
    Z3_context ctx = fctx->ctx;
    if (!_z3c_same_sort_raw(ctx, lhs, rhs)) {
        printf(
            "z3_wrapper: skip mismatched eq in %s: lhs=%s rhs=%s\n",
            where ? where : "unknown",
            Z3_sort_to_string(ctx, Z3_get_sort(ctx, lhs)),
            Z3_sort_to_string(ctx, Z3_get_sort(ctx, rhs))
        );
        fflush(stdout);
        return NULL;
    }

    return Z3_mk_eq(ctx, lhs, rhs);
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
            if (!zero) return NULL;
            Z3_ast eq = _z3c_mk_eq_checked_raw(fctx, value, zero, "_z3c_as_bool");
            return eq ? Z3_mk_not(ctx, eq) : NULL;
        }
        default: return NULL;
    }
}

static Z3_ast _z3c_coerce(z3_func_ctx_t* fctx, Z3_ast value, Z3_sort target, int is_signed) {
    if (!value || !target) return NULL;
    Z3_context ctx = fctx->ctx;
    Z3_sort source = Z3_get_sort(ctx, value);
    if (Z3_is_eq_sort(ctx, source, target)) return value;

    Z3_sort_kind source_kind = Z3_get_sort_kind(ctx, source);
    Z3_sort_kind target_kind = Z3_get_sort_kind(ctx, target);

    if (source_kind == Z3_BOOL_SORT && target_kind == Z3_BV_SORT) {
        return Z3_mk_ite(ctx, value, Z3_mk_int(ctx, 1, target), Z3_mk_int(ctx, 0, target));
    }

    if (source_kind == Z3_BOOL_SORT && target_kind == Z3_INT_SORT) {
        return Z3_mk_ite(ctx, value, Z3_mk_int(ctx, 1, target), Z3_mk_int(ctx, 0, target));
    }

    if (source_kind == Z3_BOOL_SORT && target_kind == Z3_REAL_SORT) {
        return Z3_mk_ite(ctx, value, Z3_mk_numeral(ctx, "1", target), Z3_mk_numeral(ctx, "0", target));
    }

    if (source_kind == Z3_BV_SORT && target_kind == Z3_BOOL_SORT) {
        return _z3c_as_bool(fctx, value);
    }

    if (source_kind == Z3_INT_SORT && target_kind == Z3_BOOL_SORT) {
        return _z3c_as_bool(fctx, value);
    }

    if (source_kind == Z3_REAL_SORT && target_kind == Z3_BOOL_SORT) {
        return _z3c_as_bool(fctx, value);
    }

    if (source_kind == Z3_INT_SORT && target_kind == Z3_REAL_SORT) {
        return Z3_mk_int2real(ctx, value);
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

static int _z3c_align_eq(z3_func_ctx_t* fctx, Z3_ast* lhs, Z3_ast* rhs, int lhs_signed, int rhs_signed) {
    if (!lhs || !rhs || !*lhs || !*rhs) return 0;
    Z3_context ctx = fctx->ctx;
    Z3_sort lhs_sort = Z3_get_sort(ctx, *lhs);
    Z3_sort rhs_sort = Z3_get_sort(ctx, *rhs);
    if (_z3c_same_sort_raw(ctx, *lhs, *rhs)) return 1;

    Z3_sort_kind lhs_kind = Z3_get_sort_kind(ctx, lhs_sort);
    Z3_sort_kind rhs_kind = Z3_get_sort_kind(ctx, rhs_sort);

    if (lhs_kind == Z3_BV_SORT && rhs_kind == Z3_BV_SORT) {
        unsigned lhs_bits = Z3_get_bv_sort_size(ctx, lhs_sort);
        unsigned rhs_bits = Z3_get_bv_sort_size(ctx, rhs_sort);
        Z3_sort target = lhs_bits >= rhs_bits ? lhs_sort : rhs_sort;
        *lhs = _z3c_coerce(fctx, *lhs, target, lhs_signed);
        *rhs = _z3c_coerce(fctx, *rhs, target, rhs_signed);
        return *lhs && *rhs;
    }

    if (lhs_kind == Z3_BOOL_SORT && rhs_kind != Z3_BOOL_SORT) {
        *lhs = _z3c_coerce(fctx, *lhs, rhs_sort, lhs_signed);
        return *lhs != NULL;
    }

    if (rhs_kind == Z3_BOOL_SORT && lhs_kind != Z3_BOOL_SORT) {
        *rhs = _z3c_coerce(fctx, *rhs, lhs_sort, rhs_signed);
        return *rhs != NULL;
    }

    if (lhs_kind == Z3_INT_SORT && rhs_kind == Z3_REAL_SORT) {
        *lhs = _z3c_coerce(fctx, *lhs, rhs_sort, lhs_signed);
        return *lhs != NULL;
    }

    if (lhs_kind == Z3_REAL_SORT && rhs_kind == Z3_INT_SORT) {
        *rhs = _z3c_coerce(fctx, *rhs, lhs_sort, rhs_signed);
        return *rhs != NULL;
    }

    return 0;
}

static Z3_ast _z3c_mk_eq_safe(z3_func_ctx_t* fctx, Z3_ast lhs, Z3_ast rhs, int lhs_signed, int rhs_signed) {
    if (!_z3c_align_eq(fctx, &lhs, &rhs, lhs_signed, rhs_signed)) return NULL;

    Z3_context ctx = fctx->ctx;
    if (!_z3c_same_sort_raw(ctx, lhs, rhs)) return NULL;

    return _z3c_mk_eq_checked_raw(fctx, lhs, rhs, "_z3c_mk_eq_safe");
}

static Z3_ast _z3c_mk_ne_safe(z3_func_ctx_t* fctx, Z3_ast lhs, Z3_ast rhs, int lhs_signed, int rhs_signed) {
    Z3_ast eq = _z3c_mk_eq_safe(fctx, lhs, rhs, lhs_signed, rhs_signed);
    return eq ? Z3_mk_not(fctx->ctx, eq) : NULL;
}

static int _z3c_prepare_binary_raw(
    z3_func_ctx_t* fctx,
    int op,
    int lhs_signed,
    int rhs_signed,
    Z3_ast* lhs,
    Z3_ast* rhs
) {
    if (!lhs || !rhs || !*lhs || !*rhs) return 0;

    Z3_context ctx = fctx->ctx;

    if (_z3c_op_is_shift(op)) {
        Z3_sort lhs_sort = Z3_get_sort(ctx, *lhs);
        if (Z3_get_sort_kind(ctx, lhs_sort) == Z3_BOOL_SORT) {
            lhs_sort = Z3_mk_bv_sort(ctx, 1);
            *lhs = _z3c_coerce(fctx, *lhs, lhs_sort, lhs_signed);
        }

        if (!*lhs) return 0;
        lhs_sort = Z3_get_sort(ctx, *lhs);
        if (Z3_get_sort_kind(ctx, lhs_sort) != Z3_BV_SORT) return 0;
        *rhs = _z3c_coerce(fctx, *rhs, lhs_sort, rhs_signed);
        return *rhs != NULL;
    }

    if (_z3c_op_is_equality(op)) {
        return _z3c_align_eq(fctx, lhs, rhs, lhs_signed, rhs_signed);
    }

    Z3_sort lhs_sort = Z3_get_sort(ctx, *lhs);
    Z3_sort rhs_sort = Z3_get_sort(ctx, *rhs);
    Z3_sort_kind lhs_kind = Z3_get_sort_kind(ctx, lhs_sort);
    Z3_sort_kind rhs_kind = Z3_get_sort_kind(ctx, rhs_sort);

    if (lhs_kind == Z3_BOOL_SORT && rhs_kind == Z3_BOOL_SORT) {
        Z3_sort bv1 = Z3_mk_bv_sort(ctx, 1);
        *lhs = _z3c_coerce(fctx, *lhs, bv1, lhs_signed);
        *rhs = _z3c_coerce(fctx, *rhs, bv1, rhs_signed);
        return *lhs && *rhs;
    }

    if (lhs_kind == Z3_BV_SORT && rhs_kind == Z3_BV_SORT) {
        unsigned lhs_bits = Z3_get_bv_sort_size(ctx, lhs_sort);
        unsigned rhs_bits = Z3_get_bv_sort_size(ctx, rhs_sort);
        Z3_sort target = lhs_bits >= rhs_bits ? lhs_sort : rhs_sort;
        *lhs = _z3c_coerce(fctx, *lhs, target, lhs_signed);
        *rhs = _z3c_coerce(fctx, *rhs, target, rhs_signed);
        return *lhs && *rhs;
    }

    if (lhs_kind == Z3_BOOL_SORT) {
        *lhs = _z3c_coerce(fctx, *lhs, rhs_sort, lhs_signed);
        return *lhs != NULL;
    }

    if (rhs_kind == Z3_BOOL_SORT) {
        *rhs = _z3c_coerce(fctx, *rhs, lhs_sort, rhs_signed);
        return *rhs != NULL;
    }

    if (lhs_kind == Z3_INT_SORT && rhs_kind == Z3_REAL_SORT) {
        *lhs = _z3c_coerce(fctx, *lhs, rhs_sort, lhs_signed);
        return *lhs != NULL;
    }

    if (lhs_kind == Z3_REAL_SORT && rhs_kind == Z3_INT_SORT) {
        *rhs = _z3c_coerce(fctx, *rhs, lhs_sort, rhs_signed);
        return *rhs != NULL;
    }

    return _z3c_same_sort_raw(ctx, *lhs, *rhs);
}

static int _z3c_prepare_binary(z3_func_ctx_t* fctx, hir_block_t* h, Z3_ast* lhs, Z3_ast* rhs) {
    if (!h) return 0;
    return _z3c_prepare_binary_raw(
        fctx,
        h->op,
        _z3c_is_signed_subject(h->sarg),
        _z3c_is_signed_subject(h->targ),
        lhs,
        rhs
    );
}

static int _z3c_have_same_sort(z3_func_ctx_t* fctx, Z3_ast lhs, Z3_ast rhs) {
    if (!fctx || !lhs || !rhs) return 0;
    return _z3c_same_sort_raw(fctx->ctx, lhs, rhs);
}

static Z3_ast _z3c_mk_order_safe(z3_func_ctx_t* fctx, Z3_ast lhs, Z3_ast rhs, int lhs_signed, int rhs_signed, int op) {
    if (!_z3c_prepare_binary_raw(fctx, op, lhs_signed, rhs_signed, &lhs, &rhs)) return NULL;
    if (!_z3c_have_same_sort(fctx, lhs, rhs)) return NULL;

    Z3_context ctx = fctx->ctx;
    Z3_sort lhs_sort = Z3_get_sort(ctx, lhs);
    Z3_sort_kind kind = Z3_get_sort_kind(ctx, lhs_sort);
    int signed_op = lhs_signed && rhs_signed;

    if (kind == Z3_BV_SORT) {
        switch (op) {
            case HIR_iLRG: return signed_op ? Z3_mk_bvsgt(ctx, lhs, rhs) : Z3_mk_bvugt(ctx, lhs, rhs);
            case HIR_iLGE: return signed_op ? Z3_mk_bvsge(ctx, lhs, rhs) : Z3_mk_bvuge(ctx, lhs, rhs);
            case HIR_iLWR: return signed_op ? Z3_mk_bvslt(ctx, lhs, rhs) : Z3_mk_bvult(ctx, lhs, rhs);
            case HIR_iLRE: return signed_op ? Z3_mk_bvsle(ctx, lhs, rhs) : Z3_mk_bvule(ctx, lhs, rhs);
            default: break;
        }
    }
    else if (kind == Z3_REAL_SORT || kind == Z3_INT_SORT) {
        switch (op) {
            case HIR_iLRG: return Z3_mk_gt(ctx, lhs, rhs);
            case HIR_iLGE: return Z3_mk_ge(ctx, lhs, rhs);
            case HIR_iLWR: return Z3_mk_lt(ctx, lhs, rhs);
            case HIR_iLRE: return Z3_mk_le(ctx, lhs, rhs);
            default: break;
        }
    }

    return NULL;
}

static Z3_ast _z3c_binary(z3_func_ctx_t* fctx, hir_block_t* h) {
    Z3_ast lhs = _z3c_expr(fctx, h->sarg);
    Z3_ast rhs = _z3c_expr(fctx, h->targ);
    if (!lhs || !rhs) return NULL;

    Z3_context ctx = fctx->ctx;

    if (_z3c_op_is_logical(h->op)) {
        Z3_ast args[] = { _z3c_as_bool(fctx, lhs), _z3c_as_bool(fctx, rhs) };
        if (!args[0] || !args[1]) return NULL;
        return h->op == HIR_iAND ? Z3_mk_and(ctx, 2, args) : Z3_mk_or(ctx, 2, args);
    }

    if (!_z3c_prepare_binary(fctx, h, &lhs, &rhs)) return NULL;

    if (!_z3c_have_same_sort(fctx, lhs, rhs)) return NULL;

    int lhs_signed = _z3c_is_signed_subject(h->sarg);
    int rhs_signed = _z3c_is_signed_subject(h->targ);

    if (h->op == HIR_iCMP) {
        return _z3c_mk_eq_safe(fctx, lhs, rhs, lhs_signed, rhs_signed);
    }

    if (h->op == HIR_iNMP) {
        return _z3c_mk_ne_safe(fctx, lhs, rhs, lhs_signed, rhs_signed);
    }

    Z3_sort lhs_sort = Z3_get_sort(ctx, lhs);
    Z3_sort_kind kind = Z3_get_sort_kind(ctx, lhs_sort);
    int signed_op = lhs_signed && rhs_signed;

    if (kind == Z3_BOOL_SORT) {
        switch (h->op) {
            default: break;
        }
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
            default: break;
        }
    }

    return NULL;
}

static int _z3c_assert_assign(z3_func_ctx_t* fctx, hir_subject_t* dst_subject, Z3_ast value) {
    Z3_ast dst = _z3c_var(fctx, dst_subject);
    if (!dst || !value) return 0;
    value = _z3c_coerce(fctx, value, Z3_get_sort(fctx->ctx, dst), HIR_is_sign(dst_subject->t));
    if (!value || !_z3c_have_same_sort(fctx, dst, value)) return 0;
    Z3_ast eq = _z3c_mk_eq_safe(fctx, dst, value, HIR_is_sign(dst_subject->t), HIR_is_sign(dst_subject->t));
    if (!eq) return 0;
    Z3_solver_assert(fctx->ctx, fctx->solver, eq);
    return 1;
}

static int _z3c_lower_instruction(z3_func_ctx_t* fctx, hir_block_t* h) {
    if (!h || h->unused) return 1;
    switch (h->op) {
        case HIR_MKLB: return 1;
        case HIR_STORE: return _z3c_assert_assign(fctx, h->farg, _z3c_expr(fctx, h->sarg));
        case HIR_NOT: {
            Z3_ast value = _z3c_as_bool(fctx, _z3c_expr(fctx, h->sarg));
            return _z3c_assert_assign(fctx, h->farg, value ? Z3_mk_not(fctx->ctx, value) : NULL);
        }
        case HIR_NEG: {
            Z3_ast value = _z3c_expr(fctx, h->sarg);
            if (!value) return 0;
            Z3_sort sort = Z3_get_sort(fctx->ctx, value);
            if (Z3_get_sort_kind(fctx->ctx, sort) == Z3_BOOL_SORT) {
                value = _z3c_coerce(fctx, value, Z3_mk_bv_sort(fctx->ctx, 1), 0);
                sort = value ? Z3_get_sort(fctx->ctx, value) : NULL;
            }
            if (!value || Z3_get_sort_kind(fctx->ctx, sort) != Z3_BV_SORT) return 0;
            return _z3c_assert_assign(fctx, h->farg, Z3_mk_bvneg(fctx->ctx, value));
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
        case HIR_PHI_PREAMBLE:
        case HIR_PHI:
        case HIR_VARDECL:
        case HIR_ARRDECL:
        case HIR_STRDECL:
        case HIR_REF: case HIR_GDREF:
        case HIR_LDREF:
        case HIR_UFCLL: case HIR_FCLL: case HIR_ECLL:
        case HIR_STORE_UFCLL: case HIR_STORE_FCLL: case HIR_STORE_ECLL:
        case HIR_SYSC: case HIR_STORE_SYSC: {
            if (h->farg && HIR_is_vartype(h->farg->t)) _z3c_var(fctx, h->farg);
            return 1;
        }
        case HIR_IFOP2:
        case HIR_JMP:
        case HIR_FRET:
        case HIR_EXITOP:
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
    map_init(&fctx->vars_by_id, MAP_NO_CMP);
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
    map_free(&fctx->vars_by_id);
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
    analyzer->ctx = Z3_mk_context(analyzer->cfg);
    Z3_set_error_handler(analyzer->ctx, _z3c_error_handler);
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

static int _z3c_max_depth(cfg_func_t* function) {
    return MAX(16, _z3c_block_count(function) * 4);
}

static Z3_ast _z3c_edge_condition(z3_func_ctx_t* fctx, cfg_block_t* src, cfg_block_t* dst) {
    if (!src || !src->hmap.exit || src->hmap.exit->op != HIR_IFOP2) return Z3_mk_true(fctx->ctx);
    Z3_ast cond = _z3c_as_bool(fctx, _z3c_expr(fctx, src->hmap.exit->farg));
    if (!cond) return NULL;
    if (dst == src->l) return cond;
    if (dst == src->jmp) return Z3_mk_not(fctx->ctx, cond);
    return NULL;
}

static inline int _z3c_ast_is_true(Z3_context ctx, Z3_ast ast) {
    return ast && Z3_get_bool_value(ctx, ast) == Z3_L_TRUE;
}

static int _z3c_assert_path_preambles(z3_func_ctx_t* fctx, cfg_block_t* bb) {
    if (!fctx || !bb) return 0;
    iterate_hir_instructions (bb) {
        if (hh->op != HIR_PHI_PREAMBLE) continue;
        if (!_z3c_assert_assign(fctx, hh->farg, _z3c_expr(fctx, hh->sarg))) return 0;
    }

    return 1;
}

static int _z3c_check_current(z3_func_ctx_t* fctx, Z3_ast extra) {
    if (Z3_solver_check(fctx->ctx, fctx->solver) != Z3_L_TRUE) return 1;

    int pushed = 0;
    if (extra && !_z3c_ast_is_true(fctx->ctx, extra)) {
        Z3_solver_push(fctx->ctx, fctx->solver);
        Z3_solver_assert(fctx->ctx, fctx->solver, extra);
        pushed = 1;
    }

    Z3_lbool check = Z3_solver_check(fctx->ctx, fctx->solver);
    if (pushed) Z3_solver_pop(fctx->ctx, fctx->solver, 1);
    return check != Z3_L_FALSE;
}

static int _z3c_any_path_to_block_rec(
    z3_func_ctx_t* fctx,
    cfg_block_t* curr,
    cfg_block_t* target,
    Z3_ast extra,
    int depth,
    int max_depth,
    set_t* active
) {
    if (!curr) return 0;
    if (depth > max_depth) return 1;
    if (curr == target) return _z3c_check_current(fctx, extra);
    if (set_has(active, curr)) return 1;
    set_add(active, curr);

    cfg_block_t* succs[] = { curr->l, curr->jmp };
    for (int i = 0; i < 2; ++i) {
        cfg_block_t* succ = succs[i];
        if (!succ) continue;
        Z3_ast cond = _z3c_edge_condition(fctx, curr, succ);
        if (!cond) {
            set_remove(active, curr);
            return 1;
        }
        Z3_solver_push(fctx->ctx, fctx->solver);
        if (!_z3c_assert_path_preambles(fctx, curr)) {
            Z3_solver_pop(fctx->ctx, fctx->solver, 1);
            set_remove(active, curr);
            return 1;
        }
        if (!_z3c_ast_is_true(fctx->ctx, cond)) Z3_solver_assert(fctx->ctx, fctx->solver, cond);
        int found = _z3c_any_path_to_block_rec(fctx, succ, target, extra, depth + 1, max_depth, active);
        Z3_solver_pop(fctx->ctx, fctx->solver, 1);
        if (found) {
            set_remove(active, curr);
            return 1;
        }
    }

    set_remove(active, curr);
    return 0;
}

static int _z3c_any_path_to_block(z3_func_ctx_t* fctx, cfg_block_t* curr, cfg_block_t* target, Z3_ast extra, int depth, int max_depth) {
    set_t active;
    set_init(&active, SET_NO_CMP);
    int result = _z3c_any_path_to_block_rec(fctx, curr, target, extra, depth, max_depth, &active);
    set_free(&active);
    return result;
}

static int _z3c_any_terminal_path_rec(
    z3_func_ctx_t* fctx,
    cfg_block_t* curr,
    Z3_ast extra,
    int depth,
    int max_depth,
    set_t* active
) {
    if (!curr) return 0;
    if (depth > max_depth) return 1;
    if (!curr->l && !curr->jmp) return _z3c_check_current(fctx, extra);
    if (set_has(active, curr)) return 1;
    set_add(active, curr);

    cfg_block_t* succs[] = { curr->l, curr->jmp };
    for (int i = 0; i < 2; ++i) {
        cfg_block_t* succ = succs[i];
        if (!succ) continue;
        Z3_ast cond = _z3c_edge_condition(fctx, curr, succ);
        if (!cond) {
            set_remove(active, curr);
            return 1;
        }
        Z3_solver_push(fctx->ctx, fctx->solver);
        if (!_z3c_assert_path_preambles(fctx, curr)) {
            Z3_solver_pop(fctx->ctx, fctx->solver, 1);
            set_remove(active, curr);
            return 1;
        }
        if (!_z3c_ast_is_true(fctx->ctx, cond)) Z3_solver_assert(fctx->ctx, fctx->solver, cond);
        int found = _z3c_any_terminal_path_rec(fctx, succ, extra, depth + 1, max_depth, active);
        Z3_solver_pop(fctx->ctx, fctx->solver, 1);
        if (found) {
            set_remove(active, curr);
            return 1;
        }
    }

    set_remove(active, curr);
    return 0;
}

static int _z3c_any_terminal_path(z3_func_ctx_t* fctx, cfg_block_t* curr, Z3_ast extra, int depth, int max_depth) {
    set_t active;
    set_init(&active, SET_NO_CMP);
    int result = _z3c_any_terminal_path_rec(fctx, curr, extra, depth, max_depth, &active);
    set_free(&active);
    return result;
}

static int _z3c_predicate_state(z3_func_ctx_t* fctx, cfg_func_t* function, Z3_ast pred) {
    if (!fctx || !function || !pred) return Z3A_UNKNOWN;
    cfg_block_t* entry = (cfg_block_t*)list_get_head(&function->blocks);
    int max_depth = _z3c_max_depth(function);
    Z3_ast not_pred = Z3_mk_not(fctx->ctx, pred);
    int can_true  = _z3c_any_terminal_path(fctx, entry, pred, 0, max_depth);
    int can_false = _z3c_any_terminal_path(fctx, entry, not_pred, 0, max_depth);

    if (can_true && !can_false) return Z3A_YES;
    if (can_true && can_false)  return Z3A_MAYBE;
    if (!can_true && can_false) return Z3A_NO;
    return Z3A_UNKNOWN;
}

static int _z3c_predicate_state_at_block(z3_func_ctx_t* fctx, cfg_func_t* function, cfg_block_t* block, Z3_ast pred) {
    if (!fctx || !function || !block || !pred) return Z3A_UNKNOWN;

    cfg_block_t* entry = (cfg_block_t*)list_get_head(&function->blocks);
    int max_depth = _z3c_max_depth(function);
    Z3_ast not_pred = Z3_mk_not(fctx->ctx, pred);
    int can_true  = _z3c_any_path_to_block(fctx, entry, block, pred, 0, max_depth);
    int can_false = _z3c_any_path_to_block(fctx, entry, block, not_pred, 0, max_depth);

    if (can_true && !can_false) return Z3A_YES;
    if (can_true && can_false)  return Z3A_MAYBE;
    if (!can_true && can_false) return Z3A_NO;
    return Z3A_UNKNOWN;
}

static int _z3c_can_reach_label(z3_analyzer_t* analyzer, cfg_func_t* function, long label_id) {
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    if (!fctx || !fctx->complete) return -1;
    cfg_block_t* target = NULL;
    if (!map_get(&fctx->labels, label_id, (void**)&target) || !target) return 0;
    cfg_block_t* entry = (cfg_block_t*)list_get_head(&function->blocks);
    return _z3c_any_path_to_block(fctx, entry, target, NULL, 0, _z3c_max_depth(function));
}

static int _z3c_predicate_for_vid_value(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value, int negate) {
    return Z3A_UNKNOWN;
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    if (!fctx || !fctx->complete) return Z3A_UNKNOWN;

    Z3_ast var = _z3c_var_by_vid(fctx, v_id);
    if (!var) return Z3A_UNKNOWN;

    Z3_sort sort = Z3_get_sort(fctx->ctx, var);
    Z3_ast val = _z3c_value_for_sort(fctx, value, sort);
    if (!val) return Z3A_UNKNOWN;

    Z3_ast eq = _z3c_mk_eq_safe(fctx, var, val, 1, 1);
    if (!eq) return Z3A_UNKNOWN;
    return _z3c_predicate_state(fctx, function, negate ? Z3_mk_not(fctx->ctx, eq) : eq);
}

static int _z3c_predicate_for_subject_value(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value, int negate) {
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    if (!fctx || !fctx->complete) return Z3A_UNKNOWN;

    Z3_ast expr = _z3c_expr(fctx, subject);
    if (!expr) return Z3A_UNKNOWN;

    Z3_sort sort = Z3_get_sort(fctx->ctx, expr);
    Z3_ast val = _z3c_value_for_sort(fctx, value, sort);
    if (!val) return Z3A_UNKNOWN;

    Z3_ast eq = _z3c_mk_eq_safe(fctx, expr, val, _z3c_is_signed_subject(subject), _z3c_is_signed_subject(subject));
    if (!eq) return Z3A_UNKNOWN;
    return _z3c_predicate_state(fctx, function, negate ? Z3_mk_not(fctx->ctx, eq) : eq);
}

static int _z3c_predicate_for_subject_value_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
) {
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    if (!fctx || !fctx->complete || !block) return Z3A_UNKNOWN;

    Z3_ast expr = _z3c_expr(fctx, subject);
    if (!expr) return Z3A_UNKNOWN;

    Z3_sort sort = Z3_get_sort(fctx->ctx, expr);
    Z3_ast val = _z3c_value_for_sort(fctx, value, sort);
    if (!val) return Z3A_UNKNOWN;

    Z3_ast eq = _z3c_mk_eq_safe(fctx, expr, val, _z3c_is_signed_subject(subject), _z3c_is_signed_subject(subject));
    if (!eq) return Z3A_UNKNOWN;
    return _z3c_predicate_state_at_block(fctx, function, block, eq);
}

static int _z3c_predicate_for_subject_order_value(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    hir_subject_t* subject,
    long long value,
    int op
) {
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    if (!fctx || !fctx->complete) return Z3A_UNKNOWN;

    Z3_ast expr = _z3c_expr(fctx, subject);
    if (!expr) return Z3A_UNKNOWN;

    Z3_sort sort = Z3_get_sort(fctx->ctx, expr);
    Z3_ast val = _z3c_value_for_sort(fctx, value, sort);
    if (!val) return Z3A_UNKNOWN;

    int subject_signed = _z3c_is_signed_subject(subject);
    Z3_ast pred = _z3c_mk_order_safe(fctx, expr, val, subject_signed, subject_signed, op);
    if (!pred) return Z3A_UNKNOWN;
    return _z3c_predicate_state(fctx, function, pred);
}

static int _z3c_predicate_for_subject_order_value_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value,
    int op
) {
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    if (!fctx || !fctx->complete || !block) return Z3A_UNKNOWN;

    Z3_ast expr = _z3c_expr(fctx, subject);
    if (!expr) return Z3A_UNKNOWN;

    Z3_sort sort = Z3_get_sort(fctx->ctx, expr);
    Z3_ast val = _z3c_value_for_sort(fctx, value, sort);
    if (!val) return Z3A_UNKNOWN;

    int subject_signed = _z3c_is_signed_subject(subject);
    Z3_ast pred = _z3c_mk_order_safe(fctx, expr, val, subject_signed, subject_signed, op);
    if (!pred) return Z3A_UNKNOWN;
    return _z3c_predicate_state_at_block(fctx, function, block, pred);
}

static int _z3c_predicate_for_subjects_eq(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* lhs, hir_subject_t* rhs, int negate) {
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    if (!fctx || !fctx->complete) return Z3A_UNKNOWN;

    Z3_ast lhs_ast = _z3c_expr(fctx, lhs);
    Z3_ast rhs_ast = _z3c_expr(fctx, rhs);
    if (!lhs_ast || !rhs_ast) return Z3A_UNKNOWN;

    Z3_ast eq = _z3c_mk_eq_safe(fctx, lhs_ast, rhs_ast, _z3c_is_signed_subject(lhs), _z3c_is_signed_subject(rhs));
    if (!eq) return Z3A_UNKNOWN;
    return _z3c_predicate_state(fctx, function, negate ? Z3_mk_not(fctx->ctx, eq) : eq);
}

int Z3A_is_function_complete(z3_analyzer_t* analyzer, cfg_func_t* function) {
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    return fctx && fctx->complete;
}

int Z3_can_vid_be_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value) {
    return _z3c_predicate_for_vid_value(analyzer, function, v_id, value, 0);
}

int Z3_can_vid_be_not_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value) {
    return _z3c_predicate_for_vid_value(analyzer, function, v_id, value, 1);
}

int Z3_is_vid_always_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value) {
    return Z3_can_vid_be_equal_llong(analyzer, function, v_id, value) == Z3A_YES;
}

int Z3_is_vid_maybe_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value) {
    int r = Z3_can_vid_be_equal_llong(analyzer, function, v_id, value);
    return r == Z3A_YES || r == Z3A_MAYBE;
}

int Z3_is_vid_always_zero(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id) {
    return Z3_is_vid_always_equal_llong(analyzer, function, v_id, 0);
}

int Z3_is_vid_maybe_zero(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id) {
    return Z3_is_vid_maybe_equal_llong(analyzer, function, v_id, 0);
}

int Z3_is_vid_always_nonzero(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id) {
    return Z3_can_vid_be_equal_llong(analyzer, function, v_id, 0) == Z3A_NO;
}

int Z3_check_subject_eq_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    return _z3c_predicate_for_subject_value(analyzer, function, subject, value, 0);
}

int Z3_check_subject_eq_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
) {
    return _z3c_predicate_for_subject_value_at_block(analyzer, function, block, subject, value);
}

int Z3_check_subject_ne_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    return _z3c_predicate_for_subject_value(analyzer, function, subject, value, 1);
}

int Z3_check_subject_gt_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    return _z3c_predicate_for_subject_order_value(analyzer, function, subject, value, HIR_iLRG);
}

int Z3_check_subject_ge_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    return _z3c_predicate_for_subject_order_value(analyzer, function, subject, value, HIR_iLGE);
}

int Z3_check_subject_lt_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    return _z3c_predicate_for_subject_order_value(analyzer, function, subject, value, HIR_iLWR);
}

int Z3_check_subject_le_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    return _z3c_predicate_for_subject_order_value(analyzer, function, subject, value, HIR_iLRE);
}

int Z3_check_subject_gt_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
) {
    return _z3c_predicate_for_subject_order_value_at_block(analyzer, function, block, subject, value, HIR_iLRG);
}

int Z3_check_subject_ge_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
) {
    return _z3c_predicate_for_subject_order_value_at_block(analyzer, function, block, subject, value, HIR_iLGE);
}

int Z3_check_subject_lt_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
) {
    return _z3c_predicate_for_subject_order_value_at_block(analyzer, function, block, subject, value, HIR_iLWR);
}

int Z3_check_subject_le_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
) {
    return _z3c_predicate_for_subject_order_value_at_block(analyzer, function, block, subject, value, HIR_iLRE);
}

int Z3_check_subjects_equal(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* lhs, hir_subject_t* rhs) {
    return _z3c_predicate_for_subjects_eq(analyzer, function, lhs, rhs, 0);
}

int Z3_check_subjects_not_equal(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* lhs, hir_subject_t* rhs) {
    return _z3c_predicate_for_subjects_eq(analyzer, function, lhs, rhs, 1);
}

int Z3_check_subject_truth(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    if (!fctx || !fctx->complete) return Z3A_UNKNOWN;
    Z3_ast pred = _z3c_as_bool(fctx, _z3c_expr(fctx, subject));
    if (!pred) return Z3A_UNKNOWN;
    return _z3c_predicate_state(fctx, function, pred);
}

int Z3_is_subject_always_true(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    return Z3_check_subject_truth(analyzer, function, subject) == Z3A_YES;
}

int Z3_is_subject_maybe_true(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    int r = Z3_check_subject_truth(analyzer, function, subject);
    return r == Z3A_YES || r == Z3A_MAYBE;
}

int Z3_is_subject_always_false(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    return Z3_check_subject_truth(analyzer, function, subject) == Z3A_NO;
}

int Z3_is_subject_maybe_false(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    int r = Z3_check_subject_truth(analyzer, function, subject);
    return r == Z3A_NO || r == Z3A_MAYBE;
}

int Z3_is_subject_maybe_zero(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    int r = Z3_check_subject_eq_llong(analyzer, function, subject, 0);
    return r == Z3A_YES || r == Z3A_MAYBE;
}

int Z3_is_subject_always_zero(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    return Z3_check_subject_eq_llong(analyzer, function, subject, 0) == Z3A_YES;
}

int Z3_is_subject_always_nonzero(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    return Z3_check_subject_eq_llong(analyzer, function, subject, 0) == Z3A_NO;
}

int Z3_is_subject_always_greater_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    return Z3_check_subject_gt_llong(analyzer, function, subject, value) == Z3A_YES;
}

int Z3_is_subject_maybe_greater_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    int r = Z3_check_subject_gt_llong(analyzer, function, subject, value);
    return r == Z3A_YES || r == Z3A_MAYBE;
}

int Z3_is_subject_always_greater_or_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    return Z3_check_subject_ge_llong(analyzer, function, subject, value) == Z3A_YES;
}

int Z3_is_subject_maybe_greater_or_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    int r = Z3_check_subject_ge_llong(analyzer, function, subject, value);
    return r == Z3A_YES || r == Z3A_MAYBE;
}

int Z3_is_subject_always_less_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    return Z3_check_subject_lt_llong(analyzer, function, subject, value) == Z3A_YES;
}

int Z3_is_subject_maybe_less_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    int r = Z3_check_subject_lt_llong(analyzer, function, subject, value);
    return r == Z3A_YES || r == Z3A_MAYBE;
}

int Z3_is_subject_always_less_or_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    return Z3_check_subject_le_llong(analyzer, function, subject, value) == Z3A_YES;
}

int Z3_is_subject_maybe_less_or_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    int r = Z3_check_subject_le_llong(analyzer, function, subject, value);
    return r == Z3A_YES || r == Z3A_MAYBE;
}

int Z3_is_divisor_maybe_zero(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* divisor) {
    return Z3_is_subject_maybe_zero(analyzer, function, divisor);
}

int Z3_is_label_reachable(z3_analyzer_t* analyzer, cfg_func_t* function, long l_id) {
    int result = _z3c_can_reach_label(analyzer, function, l_id);
    if (result >= 0) return result;
    return 1;
}

int Z3_is_block_reachable(z3_analyzer_t* analyzer, cfg_func_t* function, cfg_block_t* block) {
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    if (!block) return 0;
    if (!fctx || !fctx->complete) return 1;
    cfg_block_t* entry = (cfg_block_t*)list_get_head(&function->blocks);
    return _z3c_any_path_to_block(fctx, entry, block, NULL, 0, _z3c_max_depth(function));
}

int Z3_is_edge_feasible(z3_analyzer_t* analyzer, cfg_func_t* function, cfg_block_t* src, cfg_block_t* dst) {
    z3_func_ctx_t* fctx = _z3c_get_function(analyzer, function);
    if (!src || !dst) return 0;
    if (!fctx || !fctx->complete) return 1;
    cfg_block_t* entry = (cfg_block_t*)list_get_head(&function->blocks);
    Z3_ast cond = _z3c_edge_condition(fctx, src, dst);
    if (!cond) return 1;
    return _z3c_any_path_to_block(fctx, entry, src, cond, 0, _z3c_max_depth(function));
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

int Z3A_is_function_complete(z3_analyzer_t* analyzer, cfg_func_t* function) {
    (void)analyzer; (void)function;
    return 0;
}

int Z3_can_vid_be_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value) {
    (void)analyzer; (void)function; (void)v_id; (void)value;
    return Z3A_MAYBE;
}

int Z3_can_vid_be_not_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value) {
    (void)analyzer; (void)function; (void)v_id; (void)value;
    return Z3A_MAYBE;
}

int Z3_is_vid_always_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value) {
    (void)analyzer; (void)function; (void)v_id; (void)value;
    return 0;
}

int Z3_is_vid_maybe_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id, long long value) {
    (void)analyzer; (void)function; (void)v_id; (void)value;
    return 1;
}

int Z3_is_vid_always_zero(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id) {
    (void)analyzer; (void)function; (void)v_id;
    return 0;
}

int Z3_is_vid_maybe_zero(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id) {
    (void)analyzer; (void)function; (void)v_id;
    return 1;
}

int Z3_is_vid_always_nonzero(z3_analyzer_t* analyzer, cfg_func_t* function, symbol_id_t v_id) {
    (void)analyzer; (void)function; (void)v_id;
    return 0;
}

int Z3_check_subject_eq_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return Z3A_MAYBE;
}

int Z3_check_subject_eq_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
) {
    (void)analyzer; (void)function; (void)block; (void)subject; (void)value;
    return Z3A_MAYBE;
}

int Z3_check_subject_ne_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return Z3A_MAYBE;
}

int Z3_check_subject_gt_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return Z3A_MAYBE;
}

int Z3_check_subject_ge_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return Z3A_MAYBE;
}

int Z3_check_subject_lt_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return Z3A_MAYBE;
}

int Z3_check_subject_le_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return Z3A_MAYBE;
}

int Z3_check_subject_gt_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
) {
    (void)analyzer; (void)function; (void)block; (void)subject; (void)value;
    return Z3A_MAYBE;
}

int Z3_check_subject_ge_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
) {
    (void)analyzer; (void)function; (void)block; (void)subject; (void)value;
    return Z3A_MAYBE;
}

int Z3_check_subject_lt_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
) {
    (void)analyzer; (void)function; (void)block; (void)subject; (void)value;
    return Z3A_MAYBE;
}

int Z3_check_subject_le_llong_at_block(
    z3_analyzer_t* analyzer,
    cfg_func_t* function,
    cfg_block_t* block,
    hir_subject_t* subject,
    long long value
) {
    (void)analyzer; (void)function; (void)block; (void)subject; (void)value;
    return Z3A_MAYBE;
}

int Z3_check_subjects_equal(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* lhs, hir_subject_t* rhs) {
    (void)analyzer; (void)function; (void)lhs; (void)rhs;
    return Z3A_MAYBE;
}

int Z3_check_subjects_not_equal(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* lhs, hir_subject_t* rhs) {
    (void)analyzer; (void)function; (void)lhs; (void)rhs;
    return Z3A_MAYBE;
}

int Z3_check_subject_truth(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    (void)analyzer; (void)function; (void)subject;
    return Z3A_MAYBE;
}

int Z3_is_subject_always_true(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    (void)analyzer; (void)function; (void)subject;
    return 0;
}

int Z3_is_subject_maybe_true(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    (void)analyzer; (void)function; (void)subject;
    return 1;
}

int Z3_is_subject_always_false(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    (void)analyzer; (void)function; (void)subject;
    return 0;
}

int Z3_is_subject_maybe_false(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    (void)analyzer; (void)function; (void)subject;
    return 1;
}

int Z3_is_subject_maybe_zero(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    (void)analyzer; (void)function; (void)subject;
    return 1;
}

int Z3_is_subject_always_zero(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    (void)analyzer; (void)function; (void)subject;
    return 0;
}

int Z3_is_subject_always_nonzero(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject) {
    (void)analyzer; (void)function; (void)subject;
    return 0;
}

int Z3_is_subject_always_greater_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return 0;
}

int Z3_is_subject_maybe_greater_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return 1;
}

int Z3_is_subject_always_greater_or_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return 0;
}

int Z3_is_subject_maybe_greater_or_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return 1;
}

int Z3_is_subject_always_less_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return 0;
}

int Z3_is_subject_maybe_less_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return 1;
}

int Z3_is_subject_always_less_or_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return 0;
}

int Z3_is_subject_maybe_less_or_equal_llong(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* subject, long long value) {
    (void)analyzer; (void)function; (void)subject; (void)value;
    return 1;
}

int Z3_is_divisor_maybe_zero(z3_analyzer_t* analyzer, cfg_func_t* function, hir_subject_t* divisor) {
    (void)analyzer; (void)function; (void)divisor;
    return 1;
}

int Z3_is_label_reachable(z3_analyzer_t* analyzer, cfg_func_t* function, long l_id) {
    (void)analyzer; (void)function; (void)l_id;
    return 1;
}

int Z3_is_block_reachable(z3_analyzer_t* analyzer, cfg_func_t* function, cfg_block_t* block) {
    (void)analyzer; (void)function; (void)block;
    return 1;
}

int Z3_is_edge_feasible(z3_analyzer_t* analyzer, cfg_func_t* function, cfg_block_t* src, cfg_block_t* dst) {
    (void)analyzer; (void)function; (void)src; (void)dst;
    return 1;
}
#endif

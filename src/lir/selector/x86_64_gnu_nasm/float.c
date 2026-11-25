#include <lir/selector/x86_64_gnu_nasm.h>

typedef struct {
    cfg_block_t*    bb;
    lir_block_t*    pos;
    lir_registers_t sp;
    int             word;
    int             stack_bytes;
    int             keep_bytes;
    int             preserve_flags;
    int             saved_xmm[8];
} float_fix_t;

static void _emit(float_fix_t* ctx, lir_operation_t op, lir_subject_t* a, lir_subject_t* b, lir_subject_t* c) {
    /* Each inserted instruction owns its operands, including scratch operands
       reused by several instructions in a legalization sequence. */
    if (a && a->home) a = LIR_copy_subject(a);
    if (b && (b->home || b == a)) b = LIR_copy_subject(b);
    if (c && (c->home || c == a || c == b)) c = LIR_copy_subject(c);
    if (op == LIR_fMOV || op == LIR_fMVf) op = a->size == 4 ? LIR_MOVSS : LIR_MOVSD;
    lir_block_t* inst = LIR_create_block(op, a, b, c);
    if (ctx->bb->lmap.entry == ctx->pos) ctx->bb->lmap.entry = inst;
    LIR_insert_block_before(inst, ctx->pos);
}

static void _adjust_stack(float_fix_t* ctx, int bytes) {
    if (ctx->preserve_flags) {
        _emit(ctx, LIR_REF, LIR_SUBJ_REG(ctx->sp, ctx->word), LIR_SUBJ_OFF(ctx->sp, bytes, ctx->word), NULL);
    }
    else {
        _emit(
            ctx, bytes > 0 ? LIR_iSUB : LIR_iADD,
            LIR_SUBJ_REG(ctx->sp, ctx->word), LIR_SUBJ_REG(ctx->sp, ctx->word), LIR_SUBJ_CONST(ABS(bytes))
        );
    }
    ctx->stack_bytes += bytes;
}

static int _is_xmm(lir_subject_t* s) {
    return s && s->t == LIR_REGISTER && LIR_is_xmm_register(s->storage.reg.reg);
}

static int _is_memory(lir_subject_t* s) {
    return s && (s->t == LIR_MEMORY || s->t == LIR_GLVARIABLE);
}

/* Original SP-relative operands still address the stack before our saves. */
static lir_subject_t* _operand(float_fix_t* ctx, lir_subject_t* s) {
    lir_subject_t* copy = LIR_copy_subject(s);
    if (copy->t == LIR_MEMORY && copy->storage.var.base == ctx->sp) copy->storage.var.offset -= ctx->stack_bytes;
    return copy;
}

static lir_subject_t* _scratch(float_fix_t* ctx, int size) {
    for (int reg_index = 7; reg_index >= 0; reg_index--) {
        int used = 0;
        iterate_lir_args (lir_subject_t* s, ctx->pos, 0) {
            if (_is_xmm(s) && s->storage.reg.reg == (lir_registers_t)(XMM0 + reg_index)) used = 1;
        }
        if (used || ctx->saved_xmm[reg_index]) continue;
        _adjust_stack(ctx, 16);
        _emit(ctx, LIR_MOVDQU, LIR_SUBJ_OFF(ctx->sp, 0, 16), LIR_SUBJ_REG(XMM0 + reg_index, 16), NULL);
        ctx->saved_xmm[reg_index] = ctx->stack_bytes;
        return LIR_SUBJ_REG(XMM0 + reg_index, size);
    }
    return NULL;
}

/* Materialize scalar literals through memory before loading an SSE register. */
static void _literal(float_fix_t* ctx, lir_subject_t* dst, unsigned long long bits, int size) {
    _adjust_stack(ctx, 16);
    _emit(ctx, LIR_iMOV, LIR_SUBJ_OFF(ctx->sp, 0, 4), LIR_SUBJ_CONST((unsigned int)bits), NULL);
    if (size == 8) _emit(ctx, LIR_iMOV, LIR_SUBJ_OFF(ctx->sp, -4, 4), LIR_SUBJ_CONST((unsigned int)(bits >> 32)), NULL);
    _emit(ctx, LIR_fMOV, dst, LIR_SUBJ_OFF(ctx->sp, 0, size), NULL);
    _adjust_stack(ctx, -16);
}

static void _move(float_fix_t* ctx, lir_subject_t* dst, lir_subject_t* src) {
    if (LIR_is_defined_type(src->t)) {
        if (_is_xmm(dst)) _literal(ctx, dst, LIR_get_constant_value(src), src->size);
        else {
            lir_subject_t* tmp = _scratch(ctx, src->size);
            _literal(ctx, tmp, LIR_get_constant_value(src), src->size);
            _emit(ctx, LIR_fMOV, _operand(ctx, dst), tmp, NULL);
        }
    }
    else if (_is_memory(dst) && _is_memory(src)) {
        lir_subject_t* tmp = _scratch(ctx, dst->size);
        _emit(ctx, LIR_fMOV, tmp, _operand(ctx, src), NULL);
        _emit(ctx, LIR_fMOV, _operand(ctx, dst), tmp, NULL);
    }
    else _emit(ctx, LIR_fMOV, _operand(ctx, dst), _operand(ctx, src), NULL);
}

static void _restore(float_fix_t* ctx) {
    for (int i = 0; i < 8; i++) {
        if (!ctx->saved_xmm[i]) continue;
        _emit(ctx, LIR_MOVDQU, LIR_SUBJ_REG(XMM0 + i, 16), LIR_SUBJ_OFF(ctx->sp, ctx->saved_xmm[i] - ctx->stack_bytes, 16), NULL);
    }
    if (ctx->stack_bytes != ctx->keep_bytes) _adjust_stack(ctx, ctx->keep_bytes - ctx->stack_bytes);
}

int x86_64_gnu_nasm_validate_float(cfg_block_t* bb, lir_block_t* block, sym_table_t* smt) {
    (void)smt;
    int word = CONF_get_full_bytness();
    float_fix_t ctx = { .bb = bb, .pos = block, .word = word, .sp = RSP };
    lir_subject_t* dst = block->farg;
    lir_subject_t* src = block->sarg;
    switch (block->op) {
        case LIR_fMOV: case LIR_fMVf:
            _move(&ctx, dst, src);
            break;
        case LIR_fSTARG: {
            /* Keep the outgoing slot after the temporary scratch saves. */
            int size = MAX(word, dst->size);
            _adjust_stack(&ctx, size);
            lir_subject_t* slot = LIR_SUBJ_OFF(ctx.sp, size, dst->size);
            _move(&ctx, slot, dst);
            LIR_unload_subject(slot);
            ctx.keep_bytes = size;
            break;
        }
        case LIR_fGDREF: case LIR_fLDREF: {
            int store               = block->op == LIR_fLDREF;
            lir_subject_t* address  = store ? dst : src;
            lir_subject_t* value    = store ? src : dst;
            lir_registers_t base    = address->t == LIR_REGISTER ? address->storage.reg.reg : R15;
            if (address->t != LIR_REGISTER) _emit(&ctx, LIR_iMOV, LIR_SUBJ_REG(base, word), _operand(&ctx, address), NULL);
            lir_subject_t* memory = LIR_SUBJ_OFF(base, 0, value->size);
            if (store) _move(&ctx, memory, value);
            else _move(&ctx, value, memory);
            LIR_unload_subject(memory);
            break;
        }
        case LIR_fNEG: {
            lir_subject_t* value = _is_xmm(dst) ? LIR_copy_subject(dst) : _scratch(&ctx, dst->size);
            lir_subject_t* mask  = _scratch(&ctx, dst->size);
            _move(&ctx, value, src);
            _literal(&ctx, mask, dst->size == 4 ? 0x80000000ULL : 0x8000000000000000ULL, dst->size);
            _emit(&ctx, LIR_XORPS, value, mask, NULL);
            if (!_is_xmm(dst)) _move(&ctx, dst, value);
            break;
        }
        case LIR_ADDSS: case LIR_ADDSD: case LIR_SUBSS: case LIR_SUBSD:
        case LIR_MULSS: case LIR_MULSD: case LIR_DIVSS: case LIR_DIVSD: {
            lir_subject_t* left = _is_xmm(src) ? LIR_copy_subject(src) : _scratch(&ctx, src->size);
            if (!_is_xmm(src)) _move(&ctx, left, src);
            lir_subject_t* right;
            if (LIR_is_defined_type(block->targ->t)) {
                right = _scratch(&ctx, src->size);
                _literal(&ctx, right, LIR_get_constant_value(block->targ), src->size);
            }
            else right = _operand(&ctx, block->targ);
            _emit(&ctx, block->op, left, left, right);
            if (!LIR_subj_equals(dst, left)) _move(&ctx, dst, left);
            break;
        }
        case LIR_UCOMISS: case LIR_UCOMISD: {
            lir_subject_t* left = _is_xmm(dst) ? LIR_copy_subject(dst) : _scratch(&ctx, dst->size);
            if (!_is_xmm(dst)) _move(&ctx, left, dst);
            lir_subject_t* right;
            if (LIR_is_defined_type(src->t)) {
                right = _scratch(&ctx, dst->size);
                _literal(&ctx, right, LIR_get_constant_value(src), dst->size);
            }
            else right = _operand(&ctx, src);
            _emit(&ctx, block->op, left, right, NULL);
            ctx.preserve_flags = 1;
            break;
        }
        case LIR_CVTSI2SS: case LIR_CVTSI2SD:
        case LIR_CVTSS2SD: case LIR_CVTSD2SS:
        case LIR_CVTTSS2SI: case LIR_CVTTSD2SI: {
            int from_int = block->op == LIR_CVTSI2SS || block->op == LIR_CVTSI2SD;
            int to_int   = block->op == LIR_CVTTSS2SI || block->op == LIR_CVTTSD2SI;
            lir_subject_t* input = _operand(&ctx, src);
            if (LIR_is_defined_type(src->t)) {
                LIR_unload_subject(input);
                if (!from_int) {
                    input = _scratch(&ctx, src->size);
                    _literal(&ctx, input, LIR_get_constant_value(src), src->size);
                }
                else {
                    int size = MIN(word, MAX(4, src->size));
                    _adjust_stack(&ctx, 16);
                    _emit(&ctx, LIR_iMOV, LIR_SUBJ_OFF(ctx.sp, 0, 4), LIR_SUBJ_CONST((unsigned int)LIR_get_constant_value(src)), NULL);
                    if (size == 8) _emit(&ctx, LIR_iMOV, LIR_SUBJ_OFF(ctx.sp, -4, 4), LIR_SUBJ_CONST((unsigned int)(LIR_get_constant_value(src) >> 32)), NULL);
                    input = LIR_SUBJ_OFF(ctx.sp, 0, size);
                }
            }
            lir_subject_t* output = dst;
            if (to_int) {
                if (dst->t != LIR_REGISTER || dst->size < 4) output = LIR_SUBJ_REG(R15, MAX(4, dst->size));
            }
            else if (!_is_xmm(dst)) output = _scratch(&ctx, dst->size);
            _emit(&ctx, block->op, output, input, NULL);
            if (output != dst) {
                if (to_int) {
                    lir_subject_t* narrowed = LIR_copy_subject(output);
                    narrowed->size = dst->size;
                    _emit(&ctx, LIR_iMOV, _operand(&ctx, dst), narrowed, NULL);
                }
                else _move(&ctx, dst, output);
            }
            break;
        }
        default: return 0;
    }
    _restore(&ctx);
    block->unused = 1;
    return 1;
}

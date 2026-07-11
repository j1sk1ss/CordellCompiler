#include <lir/selector/x86_64_macho_nasm.h>

/* If this is a regular (avaliable for a variable usage) register.
Params:
    - `r` - Register.

Returns 1 if this is a valid register. */
static inline int _is_regular_register(lir_registers_t r) {
    if (r > R15 || r < 0) return 0;
    lir_registers_t base = LIR_format_register(r, 8);
    if (base == RBP || base == RSP) return 0;
    return 1;
}

static const lir_registers_t _regular_registers[] = { RCX, RDX, RBX, RSI, RDI, R8, R9, R10, R11, R12, R13, R14 };

/* Convert color (index) value to a register.
The idea, that colors don't care about special and reserved registers (such as RSP, RBP, etc.),
that's why we need to convert it properly.
Params:
    - `color` - Color to convert.

Returns the converted register. */
static inline lir_registers_t _convert_color_to_register(long color) {
    if (color < 0 || color >= (long)(sizeof(_regular_registers) / sizeof(_regular_registers[0]))) return -1;
    return _regular_registers[color];
}

/* Update information about memory allocation in the provided lir subject.
Params:
    - `s` - The considering lir subject.
    - `smp` - Stack map for register spilling.
    - `colors` - Register allocation result.
    - `smt` - Symtable.

Returns 1 on success, otherwise 0. */
static int _update_subject_memory(lir_subject_t* s, stack_map_t* smp, map_t* colors, sym_table_t* smt) {
    variable_info_t vi;
    if (!VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) return 0;
    if (vi.vfs.glob) {
        s->t = LIR_GLVARIABLE;
        return 1;
    }
    
    long color = 0;
    vi.vmi.size = s->size;
    if (!vi.vmi.allocated) {
        if (
            colors && map_get(colors, s->storage.var.v_id, (void**)&color) &&     /* If the clor is found         */
            color >= 0 && _is_regular_register(_convert_color_to_register(color)) /* And if this a valid register */
        ) {
            vi.vmi.reg    = _convert_color_to_register(color);
            vi.vmi.offset = FIELD_NO_CHANGE;
        }
        else {
            vi.vmi.reg    = FIELD_NO_CHANGE;
            vi.vmi.offset = stack_map_alloc(ALIGN(vi.vmi.size, vi.vmi.align), smp);
        }

        VRTB_update_memory(vi.v_id, vi.vmi.offset, vi.vmi.size, vi.vmi.reg, FIELD_NO_CHANGE, &smt->v);
    }

    if (vi.vmi.offset >= 0) {
        s->t = LIR_MEMORY;
        s->storage.var.offset = vi.vmi.offset;
        s->storage.var.base   = RBP;
    }
    else if (vi.vmi.reg >= 0) {
        s->t = LIR_REGISTER;
        s->storage.reg.reg = vi.vmi.reg;
    }

    return 1;
}

/* Get the size of a token type.
Params:
    - `t` - Token type.

Returns the size of a token type depends on the target arch. */
static inline int _get_ast_type_size(token_type_t t) {
    switch (t) {
        case TMP_I64_TYPE_TOKEN: case TMP_U64_TYPE_TOKEN: case TMP_F64_TYPE_TOKEN:
        case I64_TYPE_TOKEN:     case U64_TYPE_TOKEN:     case F64_TYPE_TOKEN:     return 8;
        case TMP_I32_TYPE_TOKEN: case TMP_U32_TYPE_TOKEN: case TMP_F32_TYPE_TOKEN:
        case I32_TYPE_TOKEN:     case U32_TYPE_TOKEN:     case F32_TYPE_TOKEN:     return 4;
        case TMP_I16_TYPE_TOKEN: case TMP_U16_TYPE_TOKEN:
        case I16_TYPE_TOKEN:     case U16_TYPE_TOKEN:                              return 2;
        case TMP_I8_TYPE_TOKEN:  case TMP_U8_TYPE_TOKEN:
        case I8_TYPE_TOKEN:      case U8_TYPE_TOKEN:                               return 1;
        default: return 8;
    }
}

static unsigned long _pack_str_le(char* p, unsigned long n) {
    unsigned long x = 0;
    for (unsigned long i = 0; i < n; i++) x |= (unsigned long)p[i] << (8 * i);
    return x;
}

/* Check whether a memory stack is used in a function.
Params:
    - `fb` - Function block.

Returns 1 if memory is used, otherwise 0. */
static int _verify_memory_usage(cfg_func_t* fb) {
    foreach (cfg_block_t* bb, &fb->blocks) {
        iterate_lir_instructions (bb) {
            iterate_lir_args (lir_subject_t* arg, lh, 0) {
                if (arg->t == LIR_MEMORY) return 1;
            }
        }
    }

    return 0;
}

int x86_64_macho_nasm_memory_selection(cfg_ctx_t* cctx, map_t* colors, sym_table_t* smt) {
    stack_map_t smp;
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        stack_map_init(0, &smp);
        foreach (cfg_block_t* bb, &fb->blocks) {
            iterate_lir_instructions (bb) {
                switch (lh->op) {
                    case LIR_VRDEALL: {
                        variable_info_t vi;
                        if (
                            !VRTB_get_info_id(lh->farg->storage.cnst.value, &vi, &smt->v) || 
                            vi.vfs.glob || vi.vmi.offset == -1
                        ) lh->unused = 1;
                        // else stack_map_free(vi.vmi.offset, ALIGN(vi.vmi.size, vi.vmi.align), &smp);
                        break;
                    }
                    case LIR_STRDECL: {
                        variable_info_t vi;
                        if (!VRTB_get_info_id(lh->farg->storage.var.v_id, &vi, &smt->v)) break;
                        if (vi.vfs.glob) break;

                        str_info_t si;
                        array_info_t ai;
                        if (
                            STTB_get_info_id(lh->sarg->storage.str.sid, &si, &smt->s) &&
                            ARTB_get_info(lh->farg->storage.var.v_id, &ai, &smt->a)
                        ) {
                            int str_off = stack_map_alloc(ALIGN(ai.size, vi.vmi.align), &smp);
                            VRTB_update_memory(lh->farg->storage.var.v_id, str_off, ai.size, vi.vmi.reg, FIELD_NO_CHANGE, &smt->v);
                            
                            int curr_offset = str_off;
                            unsigned long block_size  = 4;
                            unsigned long  string_pos = 0;

                            while (block_size > 0) {
                                while (string_pos + block_size <= si.value->size) {
                                    LIR_insert_block_before(
                                        LIR_create_block(
                                            LIR_aMOV, 
                                            LIR_SUBJ_OFF(RBP, curr_offset, block_size), 
                                            LIR_SUBJ_CONST(_pack_str_le(si.value->body + string_pos, block_size)), NULL
                                        ), lh
                                    );

                                    curr_offset -= block_size;
                                    string_pos += block_size;
                                }

                                block_size /= 2;
                            }

                            LIR_insert_block_before(LIR_create_block(LIR_aMOV, LIR_SUBJ_OFF(RBP, curr_offset, 1), LIR_SUBJ_CONST(0), NULL), lh);
                        }

                        lh->unused = 1;
                        break;
                    }
                    case LIR_ARRDECL: {
                        variable_info_t vi;
                        if (!VRTB_get_info_id(lh->farg->storage.var.v_id, &vi, &smt->v)) break;
                        if (vi.vfs.glob) break;

                        array_info_t ai;
                        if (ARTB_get_info(lh->farg->storage.var.v_id, &ai, &smt->a)) {
                            if (ai.vla) { // TODO: VLA
                                lh->op = LIR_VLADECL;
                                _update_subject_memory(lh->farg, &smp, colors, smt);
                                _update_subject_memory(lh->sarg, &smp, colors, smt);
                                break;
                            }
                            else {
                                long reserve_size = TPTB_get_memory_size_id(vi.t_id, &smt->t);
                                if (reserve_size == FIELD_NO_CHANGE) {
                                    int el_size  = ai.elements_info.el_flags.ptr ? 8 : _get_ast_type_size(ai.elements_info.el_type);
                                    reserve_size = ai.size * el_size;
                                }

                                int arr_off = stack_map_alloc(ALIGN(reserve_size, vi.vmi.align), &smp);
                                VRTB_update_memory(lh->farg->storage.var.v_id, arr_off, reserve_size, vi.vmi.reg, FIELD_NO_CHANGE, &smt->v);

                                long el_pos = 0;
                                foreach (lir_subject_t* elem, &lh->targ->storage.list.h) {
                                    if (elem->t == LIR_VARIABLE) _update_subject_memory(elem, &smp, colors, smt);

                                    long el_offset = 0, el_size = 0, __dummy = 0;
                                    if (!TPTB_find_type_init_slot(vi.t_id, el_pos, 0, &__dummy, &el_offset, &el_size, &smt->t)) break;
                                    LIR_insert_block_before(
                                        LIR_create_block(LIR_aMOV, LIR_SUBJ_OFF(RBP, arr_off - el_offset, el_size), elem, NULL), lh
                                    );

                                    el_pos++;
                                }
                            }
                        }

                        lh->unused = 1;
                        break;
                    }
                    default: {
                        iterate_lir_args (lir_subject_t* arg, lh, 0) {
                            if (arg->t != LIR_VARIABLE) continue;
                            _update_subject_memory(arg, &smp, colors, smt);
                        }

                        break;
                    }
                }
            }
        }

        /* Save the largest offset in this function for further
           memory allocation in ASM phase. */
        if (
            fb->lmap.entry->op == LIR_FDCL || 
            fb->lmap.entry->op == LIR_STRT
        ) {
            func_info_t fi;
            if (!FNTB_get_info_id(fb->lmap.entry->farg->storage.str.sid, &fi, &smt->f)) return 0;
            if (smp.last_offset || _verify_memory_usage(fb)) fb->lmap.entry->sarg = LIR_SUBJ_CONST(smp.last_offset);
            else FNTB_update_func(fb->lmap.entry->farg->storage.str.sid, FNTB_ONLY_FLAGS(FNTB_SET_NAKED(fi.flags.naked == 1 ? 1 : 2)), &smt->f);
        }
    }

    return 1;
}

/* We need to be sure that all movs are proper. For example, we can't
preserve some instructions that aren't valid in our architecture such
as 'mov sil, r15' or 'mov r15, sil', etc. 
Params:
    - `bb` - Current base block.
    - `smt` - Symtable.

Returns 1 if the operation succeeds, otherwise 0. */
static int _validate_size_movs(cfg_block_t* bb, sym_table_t* smt) {
    iterate_lir_instructions (bb) {
        if (
            (lh->farg && lh->farg->t != LIR_MEMORY) &&
            (lh->sarg && lh->sarg->t != LIR_NUMBER && lh->sarg->t != LIR_CONSTVAL)
        ) {
            switch (lh->op) {
                case LIR_iMOV: case LIR_aMOV: case LIR_fMOV: case LIR_phiMOV: {
                    lh->op = x86_64_macho_nasm_get_proper_mov(lh->farg, lh->sarg, smt, lh->op);
                    break;
                }
                default: break;
            }
        }
    }

    return 1;
}

static lir_block_t* _create_push_arg_mov(lir_subject_t* dst, lir_subject_t* src, sym_table_t* smt) {
    lir_operation_t movop = LIR_iMOV;
    lir_subject_t* movdst = dst;

    if (
        src->size == 4 && !x86_64_macho_nasm_is_sign_type(src, smt) &&
        src->t != LIR_NUMBER && src->t != LIR_CONSTVAL
    ) {
        movdst = LIR_copy_subject(dst);
        movdst->size = 4;
    }
    else {
        movop = x86_64_macho_nasm_get_proper_mov(dst, src, smt, LIR_iMOV);
    }

    return LIR_create_block(movop, movdst, src, NULL);
}

/* After the memory selection we should be sure that this LIR is valid. 
Valid LIR implies that there is no wrong instructions such as movs "from mem to mem", 
ops "mem with mem", etc.
In a nutshell, this function doesn't do anything special. It just adds additional movs to 
temporary registers before critical operations.
Params:
    - `bb` - Current base block.
    - `smt` - Symtable.

Returns 1 if the operation succeeds, otherwise 0. */
static int _validate_selected_instuction(cfg_block_t* bb, sym_table_t* smt) {
    iterate_lir_instructions (bb) {
        list_t fixes, post_fixes;
        list_init(&fixes);
        list_init(&post_fixes);
        if (lh->farg) {
            switch (lh->op) {
                case LIR_PUSH: {
                    if (lh->farg->size == 8) break;
                    lir_subject_t* tmp = x86_64_macho_nasm_create_tmp(R15, lh->farg, smt, 8);
                    list_add(&fixes, _create_push_arg_mov(tmp, lh->farg, smt));
                    lh->farg = tmp;
                    break;
                }
                case LIR_POP: {
                    if (lh->farg->size == 8) break;
                    lir_subject_t* dst = lh->farg;
                    lir_subject_t* tmp = x86_64_macho_nasm_create_tmp(R15, dst, smt, 8);
                    lir_subject_t* narrowed = LIR_copy_subject(tmp);
                    narrowed->size = dst->size;

                    lh->farg = tmp;
                    list_add(&post_fixes, LIR_create_block(LIR_iMOV, dst, narrowed, NULL));
                    break;
                }
                default: break;
            }
        }

        if (lh->farg && lh->sarg) {
            switch (lh->op) {
                case LIR_REF:
                case LIR_REF_GDREF: {
                    if (lh->farg->t == LIR_REGISTER) break;
                    lir_subject_t* tmp = x86_64_macho_nasm_create_tmp(R15, lh->sarg, smt, 8);
                    list_add(&fixes, LIR_create_block(lh->op, tmp, lh->sarg, NULL));
                    lh->sarg = tmp;
                    lh->op   = LIR_iMOV;
                    break;
                }
                case LIR_CVTSS2SD: case LIR_CVTSD2SS: case LIR_CVTTSS2SI: case LIR_CVTTSD2SI:
                case LIR_CVTSI2SS: case LIR_CVTSI2SD:
                case LIR_MOVSX:    case LIR_MOVZX:    case LIR_phiMOV:
                case LIR_iMOV:     case LIR_aMOV:     case LIR_fMOV: {
                    if (lh->farg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) break;
                    lir_subject_t* tmp = x86_64_macho_nasm_create_tmp(R15, lh->sarg, smt, lh->farg->size);
                    list_add(&fixes, LIR_create_block(lh->op, tmp, lh->sarg, NULL));
                    lh->sarg = tmp;
                    lh->op   = LIR_iMOV;
                    break;
                }
                case LIR_LDREF: {
                    if (lh->farg->t != LIR_REGISTER) {
                        lir_subject_t* src = x86_64_macho_nasm_create_tmp(RAX, lh->farg, smt, lh->farg->size);
                        list_add(&fixes, LIR_create_block(LIR_iMOV, src, lh->farg, NULL));
                        lh->farg = x86_64_macho_nasm_create_tmp(RAX, src, smt, lh->sarg->size);
                    }

                    if (lh->sarg->t != LIR_REGISTER && lh->sarg->t != LIR_NUMBER && lh->sarg->t != LIR_CONSTVAL) {
                        lir_subject_t* src = x86_64_macho_nasm_create_tmp(R15, lh->sarg, smt, lh->sarg->size);
                        list_add(&fixes, LIR_create_block(LIR_iMOV, src, lh->sarg, NULL));
                        lh->sarg = x86_64_macho_nasm_create_tmp(R15, src, smt, lh->sarg->size);
                    }

                    if (lh->sarg->t == LIR_REGISTER) lh->sarg->size = lh->farg->dsize;
                    break;
                }
                case LIR_GDREF: {
                    if (lh->farg->t == LIR_REGISTER && lh->sarg->t != LIR_MEMORY) break;
                    lir_subject_t* src = x86_64_macho_nasm_create_tmp(R15, lh->sarg, smt, lh->sarg->size);
                    list_add(&fixes, LIR_create_block(LIR_iMOV, src, lh->sarg, NULL));
                    lir_subject_t* tmp = x86_64_macho_nasm_create_tmp(R15, lh->farg, smt, lh->farg->size);
                    list_add(&fixes, LIR_create_block(LIR_GDREF, tmp, src, NULL));
                    lh->sarg = tmp;
                    lh->op   = LIR_iMOV;
                    break;
                }
                default: break;
            }

        }
        
        if (list_size(&fixes)) {
            foreach (lir_block_t* fix, &fixes) {
                if (bb->lmap.entry == lh) bb->lmap.entry = fix;
                LIR_insert_block_before(fix, lh);
            }
        }

        foreach (lir_block_t* fix, &post_fixes) {
            if (bb->lmap.exit == lh) bb->lmap.exit = fix;
            LIR_insert_block_after(fix, lh);
        }

        list_free(&fixes);
        list_free(&post_fixes);
    }

    return 1;
}

typedef struct {
    map_t block_alignment;
} stack_alignment_ctx_t;

static inline long _stack_alignment_mod(long alignment) {
    long mod = alignment % 16;
    return mod < 0 ? mod + 16 : mod;
}

static inline int _remember_stack_alignment(cfg_block_t* bb, long alignment, stack_alignment_ctx_t* ctx) {
    long curr_alignment = _stack_alignment_mod(alignment);
    long prev_alignment = 0;
    if (map_get(&ctx->block_alignment, bb->id, (void**)&prev_alignment)) {
        return prev_alignment == curr_alignment;
    }

    return map_put(&ctx->block_alignment, bb->id, (void*)curr_alignment);
}

static inline int _is_rsp_adjustment(lir_block_t* lh, lir_operation_t op) {
    return (
        lh && lh->op == op &&
        lh->sarg && lh->sarg->t == LIR_REGISTER && LIR_format_register(lh->sarg->storage.reg.reg, 8) == RSP &&
        lh->targ && lh->targ->t == LIR_CONSTVAL && lh->targ->storage.cnst.value == 8
    );
}

static inline int _has_stack_alignment_fix(cfg_block_t* bb, lir_block_t* lh) {
    if (!bb || !lh || lh == bb->lmap.entry || lh == bb->lmap.exit) return 0;
    return _is_rsp_adjustment(lh->prev, LIR_iSUB) && _is_rsp_adjustment(lh->next, LIR_iADD);
}

static cfg_dfs_action_t _validate_stack_alignment(cfg_block_t* bb, long pred, long* alignment, stack_alignment_ctx_t* ctx, sym_table_t* smt) {
    (void)pred;
    if (!_remember_stack_alignment(bb, *alignment, ctx)) return CFG_DFS_STOP;
    iterate_lir_instructions (bb) {
        switch (lh->op) {
            case LIR_PUSH: *alignment += 8; break;
            case LIR_POP:  *alignment -= 8; break;
            case LIR_ECLL:
            case LIR_FCLL: {
                func_info_t fi;
                if (
                    !FNTB_get_info_id(lh->farg->storage.str.sid, &fi, &smt->f) || !fi.flags.abi ||
                    !_stack_alignment_mod(*alignment) || _has_stack_alignment_fix(bb, lh)
                ) break;
                lir_block_t* pre  = LIR_create_block(LIR_iSUB, LIR_SUBJ_REG(RSP, 8), LIR_SUBJ_REG(RSP, 8), LIR_SUBJ_CONST(8));
                lir_block_t* post = LIR_create_block(LIR_iADD, LIR_SUBJ_REG(RSP, 8), LIR_SUBJ_REG(RSP, 8), LIR_SUBJ_CONST(8));
                if (!pre || !post) return CFG_DFS_STOP;
                if (bb->lmap.entry == lh) bb->lmap.entry = pre;
                if (bb->lmap.exit == lh)  bb->lmap.exit = post;
                if (
                    !LIR_insert_block_before(pre, lh) ||
                    !LIR_insert_block_after(post, lh)
                ) return CFG_DFS_STOP;
                break;
            }
            default: break;
        }
    }

    return CFG_DFS_CONTINUE;
}

int x86_64_macho_nasm_memory_validation(cfg_ctx_t* cctx, sym_table_t* smt) {
    foreach (cfg_func_t* fb, &cctx->funcs) {
        func_info_t fi;
        if (!FNTB_get_info_id(fb->f_id, &fi, &smt->f)) continue;
        foreach (cfg_block_t* bb, &fb->blocks) {
            _validate_selected_instuction(bb, smt);
            _validate_size_movs(bb, smt);
            if (fi.flags.naked != 1) continue;
            iterate_lir_instructions (bb) {
                iterate_lir_args (lir_subject_t* s, lh, 0) {
                    if (
                        s->t == LIR_MEMORY && 
                        s->storage.var.base == RBP
                    ) s->storage.var.base = RSP;
                }
            }
        }

        if (fi.flags.naked == 1) continue;
        stack_alignment_ctx_t sactx;
        if (!map_init(&sactx.block_alignment, MAP_NO_CMP)) return 0;

        long stack_alignment = fb->lmap.entry->sarg ? ALIGN(fb->lmap.entry->sarg->storage.cnst.value, 16) : 8;
        int stack_alignment_valid = CFG_DFS_WALK_STATE(
            list_get_head(&fb->blocks), long, stack_alignment, _validate_stack_alignment, &sactx, smt
        );

        map_free(&sactx.block_alignment);
        if (!stack_alignment_valid) return 0;
    }

    return 1;
}

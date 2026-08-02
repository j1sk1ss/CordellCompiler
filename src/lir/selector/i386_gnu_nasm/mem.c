#include <lir/selector/i386_gnu_nasm.h>

/* If this is a regular (avaliable for a variable usage) register.
Params:
    - `r` - Register.

Returns 1 if this is a valid register. */
static inline int _is_regular_register(lir_registers_t r) {
    if (r > ESP || r < 0) return 0;
    lir_registers_t base = LIR_format_register(r, 4);
    if (base == EBP || base == ESP) return 0;
    return 1;
}

static const lir_registers_t _regular_registers[] = { EBX, EDX };

/* Convert color (index) value to a register.
The idea, that colors don't care about special and reserved registers (such as ESP, EBP, etc.),
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
        s->storage.var.base   = EBP;
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
        case I64_TYPE_TOKEN:     case U64_TYPE_TOKEN:     case F64_TYPE_TOKEN:
        case TMP_I32_TYPE_TOKEN: case TMP_U32_TYPE_TOKEN: case TMP_F32_TYPE_TOKEN:
        case I32_TYPE_TOKEN:     case U32_TYPE_TOKEN:     case F32_TYPE_TOKEN: return 4;
        case TMP_I16_TYPE_TOKEN: case TMP_U16_TYPE_TOKEN:
        case I16_TYPE_TOKEN:     case U16_TYPE_TOKEN:                          return 2;
        case TMP_I8_TYPE_TOKEN:  case TMP_U8_TYPE_TOKEN:
        case I8_TYPE_TOKEN:      case U8_TYPE_TOKEN:                           return 1;
        default: return 4;
    }
}

/* Pack up to `sizeof(unsigned long)` bytes from `p` into an integer using
little-endian byte order.
Params:
    - `p` - Source byte buffer.
    - `n` - Number of bytes to pack.

Returns the packed integer value. */
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

int i386_gnu_nasm_memory_selection(cfg_ctx_t* cctx, map_t* colors, sym_table_t* smt) {
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
                        // FIXME: Non-DFS deallocation breaks current layout
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
                                            LIR_SUBJ_OFF(EBP, curr_offset, block_size), 
                                            LIR_SUBJ_CONST(_pack_str_le(si.value->body + string_pos, block_size)), NULL
                                        ), lh
                                    );

                                    curr_offset -= block_size;
                                    string_pos  += block_size;
                                }

                                block_size /= 2;
                            }

                            LIR_insert_block_before(LIR_create_block(LIR_aMOV, LIR_SUBJ_OFF(EBP, curr_offset, 1), LIR_SUBJ_CONST(0), NULL), lh);
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
                                    int el_size  = ai.elements_info.el_flags.ptr ? 4 : _get_ast_type_size(ai.elements_info.el_type);
                                    reserve_size = ai.size * el_size;
                                }

                                int arr_off = stack_map_alloc(ALIGN(reserve_size, vi.vmi.align), &smp);
                                VRTB_update_memory(lh->farg->storage.var.v_id, arr_off, reserve_size, vi.vmi.reg, FIELD_NO_CHANGE, &smt->v);

                                long el_pos = 0;
                                foreach (lir_subject_t* elem, &lh->targ->storage.list.h) {
                                    if (elem->t == LIR_VARIABLE) _update_subject_memory(elem, &smp, colors, smt);

                                    type_init_info_t elem_info = { 0 };
                                    if (!TPTB_find_type_init_slot(vi.t_id, el_pos, 0, &elem_info, &smt->t)) break;
                                    LIR_insert_block_before(
                                        LIR_create_block(LIR_aMOV, LIR_SUBJ_OFF(EBP, arr_off - elem_info.slot_off, elem_info.slot_size), elem, NULL), lh
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
                    lh->op = i386_gnu_nasm_get_proper_mov(lh->farg, lh->sarg, smt, lh->op);
                    break;
                }
                default: break;
            }
        }
    }

    return 1;
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
        list_t fixes;
        list_init(&fixes);
        if (lh->farg) {
            switch (lh->op) {
                case LIR_PUSH: {
                    if (lh->farg->t != LIR_NUMBER && lh->farg->t != LIR_CONSTVAL) break;
                    lir_subject_t* tmp = i386_gnu_nasm_create_tmp(ECX, lh->farg, smt, MAX(lh->farg->size, 2));
                    list_add(&fixes, LIR_create_block(LIR_iMOV, tmp, lh->farg, NULL));
                    lh->farg = tmp;
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
                    lir_subject_t* tmp = i386_gnu_nasm_create_tmp(EDI, lh->sarg, smt, 4);
                    list_add(&fixes, LIR_create_block(lh->op, tmp, lh->sarg, NULL));
                    lh->sarg = tmp;
                    lh->op   = LIR_iMOV;
                    break;
                }
                case LIR_CVTSS2SD: case LIR_CVTSD2SS: case LIR_CVTTSS2SI: case LIR_CVTTSD2SI:
                case LIR_CVTSI2SS: case LIR_CVTSI2SD:
                case LIR_MOVSX:    case LIR_MOVZX:    case LIR_phiMOV:    case LIR_MOVSXD:
                case LIR_iMOV:     case LIR_aMOV:     case LIR_fMOV: {
                    if (lh->farg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) break;
                    lir_subject_t* tmp = i386_gnu_nasm_create_tmp(ECX, lh->sarg, smt, lh->farg->size);
                    list_add(&fixes, LIR_create_block(lh->op, tmp, lh->sarg, NULL));
                    lh->sarg = tmp;
                    lh->op   = LIR_iMOV;
                    break;
                }
                case LIR_LDREF: {
                    if (lh->farg->t != LIR_REGISTER) {
                        lir_subject_t* src = i386_gnu_nasm_create_tmp(EAX, lh->farg, smt, lh->farg->size);
                        list_add(&fixes, LIR_create_block(LIR_iMOV, src, lh->farg, NULL));
                        lh->farg = i386_gnu_nasm_create_tmp(EAX, src, smt, lh->sarg->size);
                    }

                    if (lh->sarg->t != LIR_REGISTER && lh->sarg->t != LIR_NUMBER && lh->sarg->t != LIR_CONSTVAL) {
                        lir_subject_t* src = i386_gnu_nasm_create_tmp(ECX, lh->sarg, smt, lh->sarg->size);
                        list_add(&fixes, LIR_create_block(LIR_iMOV, src, lh->sarg, NULL));
                        lh->sarg = i386_gnu_nasm_create_tmp(ECX, src, smt, lh->sarg->size);
                    }

                    if (lh->sarg->t == LIR_REGISTER) lh->sarg->size = lh->farg->dsize;
                    break;
                }
                case LIR_GDREF: {
                    if (
                        lh->farg->t == LIR_REGISTER && 
                        (
                            lh->sarg->t != LIR_MEMORY     && 
                            lh->sarg->t != LIR_GLVARIABLE &&
                            lh->sarg->t != LIR_VARIABLE
                        )
                    ) break;
                    lir_subject_t* src = i386_gnu_nasm_create_tmp(ECX, lh->sarg, smt, lh->sarg->size);
                    list_add(&fixes, LIR_create_block(LIR_iMOV, src, lh->sarg, NULL));
                    lir_subject_t* tmp = i386_gnu_nasm_create_tmp(ECX, lh->farg, smt, lh->farg->size);
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

        list_free(&fixes);
    }

    return 1;
}

typedef struct {
    map_t      block_alignment;
    set_t      broken_funcs;
    cfg_ctx_t* cctx;
} stack_alignment_ctx_t;

static inline long _stack_alignment_mod(long alignment) {
    long mod = alignment % 8;
    return mod < 0 ? mod + 8 : mod;
}

static inline int _remember_stack_alignment(cfg_block_t* bb, long alignment, stack_alignment_ctx_t* ctx) {
    long curr_alignment = _stack_alignment_mod(alignment);
    long prev_alignment = 0;
    if (map_get(&ctx->block_alignment, bb->id, (void**)&prev_alignment)) {
        return prev_alignment == curr_alignment;
    }

    return map_put(&ctx->block_alignment, bb->id, (void*)curr_alignment);
}

static inline int _is_esp_adjustment(lir_block_t* lh, lir_operation_t op) {
    return (
        lh && lh->op == op &&
        lh->sarg && lh->sarg->t == LIR_REGISTER && LIR_format_register(lh->sarg->storage.reg.reg, 4) == ESP &&
        lh->targ && lh->targ->t == LIR_CONSTVAL
    );
}

static inline int _has_stack_alignment_fix(cfg_block_t* bb, lir_block_t* lh) {
    if (!bb || !lh || lh == bb->lmap.entry || lh == bb->lmap.exit) return 0;
    return (
        _is_esp_adjustment(lh->prev, LIR_iSUB) && lh->prev->targ->storage.cnst.value == 4 &&
        _is_esp_adjustment(lh->next, LIR_iADD) && lh->next->targ->storage.cnst.value == 4
    );
}

#define IS_REGULAR_FCALL(lh) (lh && (lh->op == LIR_ECLL || lh->op == LIR_FCLL) && lh->farg && lh->farg->t == LIR_FNAME)

static inline int _call_is_abi(lir_block_t* lh, sym_table_t* smt) {
    func_info_t fi;
    return IS_REGULAR_FCALL(lh) && FNTB_get_info_id(lh->farg->storage.str.sid, &fi, &smt->f) && fi.flags.abi;
}

/* If a function has an abi call, and its aligned is broken, we
mark this function as a broken function.
Params:
- `cctx` - CFG context.
- `smt` - Symtable.
- `broken_funcs` - Output set of function to fix. 
    
Returns 1 if succeed, otherwise will return 0 */
static int _search_for_broken_funcs(cfg_ctx_t* cctx, sym_table_t* smt, set_t* broken_funcs) {
    int changed = 0;
    do {
        changed = 0;
        foreach (cfg_func_t* fb, &cctx->funcs) {
            if (
                !fb->used || 
                set_has(broken_funcs, (void*)fb->f_id)
            ) continue;
            foreach (cfg_block_t* bb, &fb->blocks) {
                iterate_lir_instructions (bb) {
                    if (
                        _call_is_abi(lh, smt) ||
                        (
                            IS_REGULAR_FCALL(lh) &&
                            set_has(broken_funcs, (void*)lh->farg->storage.str.sid)
                        )
                    ) goto _add_broken_func;
                }
            }

            if (0) {
_add_broken_func: {}
                if (!set_add(broken_funcs, (void*)fb->f_id)) return 0;
                changed = 1;
            }
        }
    } while (changed);
    return 1;
}

static cfg_dfs_action_t _validate_stack_alignment(
    cfg_block_t* bb, long pred, long* alignment, stack_alignment_ctx_t* ctx, sym_table_t* smt
) {
    (void)pred;
    if (!_remember_stack_alignment(bb, *alignment, ctx)) return CFG_DFS_STOP;
    iterate_lir_instructions (bb) {
        switch (lh->op) {
            case LIR_PUSH: *alignment += 4; break;
            case LIR_POP:  *alignment -= 4; break;
            case LIR_iSUB: {
                if (_is_esp_adjustment(lh, LIR_iSUB)) *alignment += lh->targ->storage.cnst.value;
                break;
            }
            case LIR_iADD: {
                if (_is_esp_adjustment(lh, LIR_iADD)) *alignment -= lh->targ->storage.cnst.value;
                break;
            }
            case LIR_ECLL:
            case LIR_FCLL: {
                if (
                    !(
                        _call_is_abi(lh, smt) ||
                        (
                            IS_REGULAR_FCALL(lh) &&
                            set_has(&ctx->broken_funcs, (void*)lh->farg->storage.str.sid)
                        )
                    ) ||
                    !_stack_alignment_mod(*alignment) || _has_stack_alignment_fix(bb, lh)
                ) break;
                lir_block_t* pre  = LIR_create_block(LIR_iSUB, LIR_SUBJ_REG(ESP, 4), LIR_SUBJ_REG(ESP, 4), LIR_SUBJ_CONST(4));
                lir_block_t* post = LIR_create_block(LIR_iADD, LIR_SUBJ_REG(ESP, 4), LIR_SUBJ_REG(ESP, 4), LIR_SUBJ_CONST(4));
                if (!pre || !post) return CFG_DFS_STOP;
                if (bb->lmap.entry == lh) bb->lmap.entry = pre;
                if (bb->lmap.exit == lh)  bb->lmap.exit = post;
                if (
                    !LIR_insert_block_before(pre, lh) ||
                    !LIR_insert_block_after(post, lh)
                ) return CFG_DFS_STOP;
                *alignment += 4;
                break;
            }
            default: break;
        }
    }

    return CFG_DFS_CONTINUE;
}

int i386_gnu_nasm_memory_validation(cfg_ctx_t* cctx, sym_table_t* smt) {
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
                        s->storage.var.base == EBP
                    ) s->storage.var.base = ESP;
                }
            }
        }
    }

    stack_alignment_ctx_t sactx = { .cctx = cctx };
    if (!set_init(&sactx.broken_funcs, SET_NO_CMP)) return 0;
    if (!_search_for_broken_funcs(cctx, smt, &sactx.broken_funcs)) {
        set_free(&sactx.broken_funcs);
        return 0;
    }

    foreach (cfg_func_t* fb, &cctx->funcs) {
        func_info_t fi;
        if (!FNTB_get_info_id(fb->f_id, &fi, &smt->f)) continue;
        if (fi.flags.naked == 1) continue;
        if (!map_init(&sactx.block_alignment, MAP_NO_CMP)) {
            set_free(&sactx.broken_funcs);
            return 0;
        }

        long stack_alignment = fb->lmap.entry->sarg ? ALIGN(fb->lmap.entry->sarg->storage.cnst.value, 8) : 4;
        int stack_alignment_valid = CFG_DFS_WALK_STATE(
            list_get_head(&fb->blocks), long, stack_alignment, _validate_stack_alignment, &sactx, smt
        );

        map_free(&sactx.block_alignment);
        if (!stack_alignment_valid) {
            set_free(&sactx.broken_funcs);
            return 0;
        }
    }

    set_free(&sactx.broken_funcs);
    return 1;
}

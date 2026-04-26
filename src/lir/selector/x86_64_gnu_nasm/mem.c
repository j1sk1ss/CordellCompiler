#include <lir/selector/x84_64_gnu_nasm.h>

/*
If this is a regular (avaliable for a variable usage) register.
Params:
    - `r` - Register.

Return 1 if this is a valid register.
*/
static inline int _is_regular_register(lir_registers_t r) {
    if (r > R15 || r < 0) return 0;
    lir_registers_t base = LIR_format_register(r, 8);
    if (base == RBP || base == RSP) return 0;
    return 1;
}

static const lir_registers_t _regular_registers[] = { RCX, RDX, RBX, RSI, RDI, R8, R9, R10, R11, R12, R13, R14 };

/*
Convert color (index) value to a register.
The idea, that colors don't care about special and reserved registers (such as RSP, RBP, etc.),
that's why we need to convert it properly.
Params:
    - `color` - Color to convert.

Returns the converted register.
*/
static inline lir_registers_t _convert_color_to_register(long color) {
    if (color < 0 || color >= (long)(sizeof(_regular_registers) / sizeof(_regular_registers[0]))) return -1;
    return _regular_registers[color];
}

/*
Update information about memory allocation in the provided lir subject.
Params:
    - `s` - The considering lir subject.
    - `smp` - Stack map for register spilling.
    - `colors` - Register allocation result.
    - `smt` - Symtable.

Return 1 if operation succeed. Otherwise it will return 0.
*/
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

/*
We need to be sure that all movs are proper. For example, we can't
preserve some instructions that aren't valid in our architecture such
as 'mov sil, r15' or 'mov r15, sil', etc. 
Params:
    - `bb` - Current base block.
    - `smt` - Symtable.

Returns 1 if an operation was secceed, otherwise it will returns 0.
*/
static int _validate_size_movs(cfg_block_t* bb, sym_table_t* smt) {
    lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
    while (lh) {
        if (
            (lh->farg && lh->farg->t != LIR_MEMORY) &&
            (lh->sarg && lh->sarg->t != LIR_NUMBER && lh->sarg->t != LIR_CONSTVAL)
        ) {
            switch (lh->op) {
                case LIR_iMOV: case LIR_aMOV: case LIR_fMOV: {
                    lh->op = x86_64_gnu_nasm_get_proper_mov(lh->farg, lh->sarg, smt, lh->op);
                    break;
                }
                default: break;
            }
        }

        lh = LIR_get_next(lh, bb->lmap.exit, 1);
    }

    return 1;
}

/*
Get the size of a token type.
Params:
    - `t` - Token type.

Returns the size of a token type depends on the target arch.
*/
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

/*
After the memory selection we should be sure that this LIR is valid. 
Valid LIR implies that there is no wrong instructions such as movs "from mem to mem", 
ops "mem with mem", etc.
In a nutshell, this function doesn't do anything special. It just adds additional movs to 
temporary registers before critical operations.
Params:
    - `bb` - Current base block.
    - `smt` - Symtable.

Returns 1 if an operation was secceed, otherwise it will returns 0.
*/
static int _validate_selected_instuction(cfg_block_t* bb, sym_table_t* smt) {
    lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
    while (lh) {
        list_t fixes;
        list_init(&fixes);
        if (lh->farg && lh->sarg) {
            switch (lh->op) {
                case LIR_REF:
                case LIR_REF_GDREF: {
                    if (lh->farg->t == LIR_REGISTER) break;
                    lir_subject_t* tmp = x86_64_gnu_nasm_create_tmp(R15, lh->sarg, smt, 8);
                    list_add(&fixes, LIR_create_block(lh->op, tmp, lh->sarg, NULL));
                    lh->sarg = tmp;
                    lh->op   = LIR_iMOV;
                    break;
                }
                case LIR_CVTSS2SD: case LIR_CVTSD2SS: case LIR_CVTTSS2SI: case LIR_CVTTSD2SI:
                case LIR_MOVSX:    case LIR_MOVZX:    case LIR_MOVSXD:
                case LIR_iMOV:     case LIR_aMOV:     case LIR_fMOV: {
                    if (lh->farg->t == LIR_REGISTER || lh->sarg->t == LIR_NUMBER || lh->sarg->t == LIR_CONSTVAL) break;
                    lir_subject_t* tmp = x86_64_gnu_nasm_create_tmp(R15, lh->sarg, smt, lh->farg->size);
                    list_add(&fixes, LIR_create_block(LIR_iMOV, tmp, lh->sarg, NULL));
                    lh->sarg = tmp;
                    break;
                }
                case LIR_LDREF: {
                    if (lh->farg->t != LIR_REGISTER) {
                        lir_subject_t* src = x86_64_gnu_nasm_create_tmp(RAX, lh->farg, smt, lh->farg->size);
                        list_add(&fixes, LIR_create_block(LIR_iMOV, src, lh->farg, NULL));
                        lh->farg = x86_64_gnu_nasm_create_tmp(RAX, src, smt, lh->sarg->size);
                    }

                    if (lh->sarg->t != LIR_REGISTER && lh->sarg->t != LIR_NUMBER && lh->sarg->t != LIR_CONSTVAL) {
                        lir_subject_t* src = x86_64_gnu_nasm_create_tmp(R15, lh->sarg, smt, lh->sarg->size);
                        list_add(&fixes, LIR_create_block(LIR_iMOV, src, lh->sarg, NULL));
                        lh->sarg = x86_64_gnu_nasm_create_tmp(R15, src, smt, lh->sarg->size);
                    }

                    break;
                }
                case LIR_GDREF: {
                    if (lh->farg->t == LIR_REGISTER) break;
                    lir_subject_t* src = x86_64_gnu_nasm_create_tmp(R15, lh->sarg, smt, lh->sarg->size);
                    list_add(&fixes, LIR_create_block(LIR_iMOV, src, lh->sarg, NULL));
                    lir_subject_t* tmp = x86_64_gnu_nasm_create_tmp(R15, lh->farg, smt, lh->farg->size);
                    list_add(&fixes, LIR_create_block(LIR_GDREF, tmp, src, NULL));
                    lh->sarg = tmp;
                    lh->op   = LIR_iMOV;
                    break;
                }
                default: break;
            }

            if (list_size(&fixes)) {
                foreach (lir_block_t* fix, &fixes) {
                    if (bb->lmap.entry == lh) bb->lmap.entry = fix;
                    LIR_insert_block_before(fix, lh);
                }
            }
        }

        lh = LIR_get_next(lh, bb->lmap.exit, 1);
        list_free(&fixes);
    }

    return 1;
}

static unsigned long _pack_str_le(char* p, unsigned long n) {
    unsigned long x = 0;
    for (unsigned long i = 0; i < n; i++) x |= (unsigned long)p[i] << (8 * i);
    return x;
}

int x86_64_gnu_nasm_memory_selection(cfg_ctx_t* cctx, map_t* colors, sym_table_t* smt) {
    stack_map_t smp;
    foreach (cfg_func_t* fb, &cctx->funcs) {
        if (!fb->used) continue;
        stack_map_init(0, &smp);
        foreach (cfg_block_t* bb, &fb->blocks) {
            lir_block_t* lh = LIR_get_next(bb->lmap.entry, bb->lmap.exit, 0);
            while (lh) {
                switch (lh->op) {
                    case LIR_VRDEALL: {
                        variable_info_t vi;
                        if (
                            !VRTB_get_info_id(lh->farg->storage.cnst.value, &vi, &smt->v) || 
                            vi.vfs.glob || vi.vmi.offset == -1
                        ) lh->unused = 1;
                        else stack_map_free(vi.vmi.offset, ALIGN(vi.vmi.size, vi.vmi.align), &smp);
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
                                            LIR_iMOV, 
                                            LIR_SUBJ_OFF(RBP, curr_offset, block_size), 
                                            LIR_SUBJ_CONST(_pack_str_le(si.value->body + string_pos, block_size)), NULL
                                        ), lh
                                    );

                                    curr_offset -= block_size;
                                    string_pos += block_size;
                                }

                                block_size /= 2;
                            }

                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, LIR_SUBJ_OFF(RBP, curr_offset, 1), LIR_SUBJ_CONST(0), NULL), lh);
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
                                int el_size = _get_ast_type_size(ai.elements_info.el_type);
                                if (ai.elements_info.el_flags.ptr) el_size = 8;
                                int arr_off = stack_map_alloc(ALIGN(ai.size * el_size, vi.vmi.align), &smp);
                                VRTB_update_memory(lh->farg->storage.var.v_id, arr_off, ai.size, vi.vmi.reg, FIELD_NO_CHANGE, &smt->v);

                                int el_pos = 0;
                                foreach (lir_subject_t* elem, &lh->targ->storage.list.h) {
                                    if (elem->t == LIR_VARIABLE) _update_subject_memory(elem, &smp, colors, smt);
                                    LIR_insert_block_before(
                                        LIR_create_block(LIR_iMOV, LIR_SUBJ_OFF(RBP, arr_off - el_pos * el_size, el_size), elem, NULL), lh
                                    );

                                    el_pos++;
                                }
                            }
                        }

                        lh->unused = 1;
                        break;
                    }
                    default: {
                        lir_subject_t* args[] = { lh->farg, lh->sarg, lh->targ };
                        for (int i = 0; i < 3; i++) {
                            if (!args[i] || args[i]->t != LIR_VARIABLE) continue;
                            _update_subject_memory(args[i], &smp, colors, smt);
                        }

                        break;
                    }
                }

                lh = LIR_get_next(lh, bb->lmap.exit, 1);
            }

            _validate_selected_instuction(bb, smt);
            _validate_size_movs(bb, smt);
        }

        /* Save the largest offset in this function for further
           memory allocation in ASM phase. */
        if (
            fb->lmap.entry->op == LIR_FDCL || 
            fb->lmap.entry->op == LIR_STRT
        ) fb->lmap.entry->sarg = LIR_SUBJ_CONST(smp.last_offset);
    }

    return 1;
}

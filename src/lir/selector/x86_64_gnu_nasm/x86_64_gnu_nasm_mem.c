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
    if (
        base == RBP || 
        base == RSP
    ) return 0;
    return 1;
}

static const lir_registers_t _regular_registers[] = { RAX, RCX, RDX, RBX, RSI, RDI, R8, R9, R10, R11, R12, R13, R14 };

/*
Convert color (index) value to a register.
The idea, that colors don't care about special and reserved registers (such as RSP, RBP, etc.),
that's why we need to convert it properly.
Params:
    - `color` - Color to convert.

Returns the converted register.
*/
static inline lir_registers_t _convert_color_to_register(long color) {
    if (color < 0 || color > (long)(sizeof(_regular_registers) / sizeof(_regular_registers[0]))) return -1;
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
                            int arroff = stack_map_alloc(ALIGN(ai.size, vi.vmi.align), &smp);
                            VRTB_update_memory(lh->farg->storage.var.v_id, arroff, ai.size, vi.vmi.reg, FIELD_NO_CHANGE, &smt->v);
                            char* string = si.value->body;
                            while (*string) {
                                LIR_insert_block_before(
                                    LIR_create_block(LIR_iMOV, LIR_SUBJ_OFF(RBP, arroff--, 1), LIR_SUBJ_CONST(*(string++)), NULL), lh
                                );
                            }

                            LIR_insert_block_before(LIR_create_block(LIR_iMOV, LIR_SUBJ_OFF(RBP, arroff, 1), LIR_SUBJ_CONST(0), NULL), lh);
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
                                int arr_off = stack_map_alloc(ALIGN(ai.size * el_size, vi.vmi.align), &smp);
                                VRTB_update_memory(lh->farg->storage.var.v_id, arr_off, ai.size, vi.vmi.reg, FIELD_NO_CHANGE, &smt->v);

                                int el_pos = 0;
                                foreach (lir_subject_t* elem, &lh->targ->storage.list.h) {
                                    if (elem->t == LIR_VARIABLE) _update_subject_memory(elem, &smp, colors, smt);
                                    LIR_insert_block_before(
                                        LIR_create_block(LIR_iMOV, LIR_SUBJ_OFF(RBP, arr_off - el_pos * el_size, 1), elem, NULL), lh
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

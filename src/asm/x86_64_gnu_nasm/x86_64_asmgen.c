#include <asm/x86_64_asmgen.h>

static int _convert_lirblock_to_assembly(lir_block_t* b, func_info_t* fi, sym_table_t* smt, FILE* output) {
    switch (b->op) {
        case LIR_FCLL:
        case LIR_ECLL: EMIT_COMMAND("call %s\n", format_lir_subject(b->farg, smt)); break;
        case LIR_STRT:
        case LIR_FDCL: {
            EMIT_COMMAND("%s:\n", format_lir_subject(b->farg, smt));
            if (!fi->flags.naked) {
                EMIT_COMMAND("push rbp\n");
                EMIT_COMMAND("mov rbp, rsp\n");
                if (b->sarg->storage.cnst.value > 0) {
                    EMIT_COMMAND("sub rsp, %ld\n", ALIGN(b->sarg->storage.cnst.value, 8));
                }
            }

            break;
        }
        case LIR_STEND:
        case LIR_EXITOP: {
            if (!fi->flags.naked) {
                EMIT_COMMAND("mov rax, 0x2000001");
                EMIT_COMMAND("syscall");
            }

            break;
        }
        case LIR_FEND:
        case LIR_FRET: {
            if (!fi->flags.naked) {
                EMIT_COMMAND("mov rsp, rbp\n");
                EMIT_COMMAND("pop rbp\n");
            }
            
            EMIT_COMMAND("ret\n");
            break;
        }
        case LIR_CDQ:  EMIT_COMMAND("cdq\n");     break;
        case LIR_SYSC: EMIT_COMMAND("syscall\n"); break;
        case LIR_FEXT: {
            func_info_t fi;
            if (FNTB_get_info_id(b->farg->storage.cnst.value, &fi, &smt->f)) {
                EMIT_COMMAND("extern %s\n", fi.name->body);
            }

            break;
        }
        case LIR_OEXT: {
            variable_info_t vi;
            if (VRTB_get_info_id(b->farg->storage.cnst.value, &vi, &smt->v)) {
                EMIT_COMMAND("extern %s\n", vi.name->body); 
            }

            break;
        }
        case LIR_BREAKPOINT: EMIT_COMMAND("int3 ; %s", format_lir_subject(b->farg, smt));                                 break;
        case LIR_BB:         EMIT_COMMAND("\n; BB%ld:", b->farg->storage.cnst.value);                                     break;
        case LIR_TST:  EMIT_COMMAND("test %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));   break;
        case LIR_XCHG: EMIT_COMMAND("xchg %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));   break;
        case LIR_MKLB: EMIT_COMMAND("%s:", format_lir_subject(b->farg, smt));                                             break;
        case LIR_SETL: EMIT_COMMAND("setl %s", format_lir_subject(b->farg, smt));                                         break;
        case LIR_SETG: EMIT_COMMAND("setg %s", format_lir_subject(b->farg, smt));                                         break;
        case LIR_STLE: EMIT_COMMAND("setle %s", format_lir_subject(b->farg, smt));                                        break;
        case LIR_STGE: EMIT_COMMAND("setge %s", format_lir_subject(b->farg, smt));                                        break;
        case LIR_SETE: EMIT_COMMAND("sete %s", format_lir_subject(b->farg, smt));                                         break;
        case LIR_STNE: EMIT_COMMAND("setne %s", format_lir_subject(b->farg, smt));                                        break;
        case LIR_SETB: EMIT_COMMAND("setb %s", format_lir_subject(b->farg, smt));                                         break;
        case LIR_SETA: EMIT_COMMAND("seta %s", format_lir_subject(b->farg, smt));                                         break;
        case LIR_STBE: EMIT_COMMAND("setbe %s", format_lir_subject(b->farg, smt));                                        break;
        case LIR_STAE: EMIT_COMMAND("setae %s", format_lir_subject(b->farg, smt));                                        break;
        case LIR_NEG: EMIT_COMMAND("neg %s", format_lir_subject(b->farg, smt));                                           break;
        case LIR_INC: EMIT_COMMAND("inc %s", format_lir_subject(b->farg, smt));                                           break;
        case LIR_DEC: EMIT_COMMAND("dec %s", format_lir_subject(b->farg, smt));                                           break;
        case LIR_JMP:  EMIT_COMMAND("jmp %s", format_lir_subject(b->farg, smt));                                          break;
        case LIR_JE:   EMIT_COMMAND("je %s", format_lir_subject(b->farg, smt));                                           break;
        case LIR_JLE:  EMIT_COMMAND("jle %s", format_lir_subject(b->farg, smt));                                          break;
        case LIR_JNE:  EMIT_COMMAND("jne %s", format_lir_subject(b->farg, smt));                                          break;
        case LIR_JL:   EMIT_COMMAND("jl %s", format_lir_subject(b->farg, smt));                                           break;
        case LIR_JG:   EMIT_COMMAND("jg %s", format_lir_subject(b->farg, smt));                                           break;
        case LIR_JGE:  EMIT_COMMAND("jge %s", format_lir_subject(b->farg, smt));                                          break;
        case LIR_JA:   EMIT_COMMAND("ja %s", format_lir_subject(b->farg, smt));                                           break;
        case LIR_JAE:  EMIT_COMMAND("jae %s", format_lir_subject(b->farg, smt));                                          break;
        case LIR_JB:   EMIT_COMMAND("jb %s", format_lir_subject(b->farg, smt));                                           break;
        case LIR_JBE:  EMIT_COMMAND("jbe %s", format_lir_subject(b->farg, smt));                                          break;
        case LIR_aMOV:
        case LIR_iMOV:  EMIT_COMMAND("mov %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));   break;
        case LIR_MOVZX: EMIT_COMMAND("movzx %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt)); break;
        case LIR_MOVSX: EMIT_COMMAND("movsx %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt)); break;
        case LIR_fMOV:
        case LIR_fMVf:  EMIT_COMMAND("movsd %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt)); break;
        case LIR_REF:   EMIT_COMMAND("lea %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));   break;
        case LIR_LDREF: EMIT_COMMAND("mov [%s], %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt)); break;
        case LIR_GDREF: EMIT_COMMAND("mov %s, [%s]", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt)); break;
        case LIR_PUSH: EMIT_COMMAND("push %s", format_lir_subject(b->farg, smt));                                         break;
        case LIR_POP:  EMIT_COMMAND("pop %s", format_lir_subject(b->farg, smt));                                          break;
        case LIR_iADD: EMIT_COMMAND("add %s, %s", format_lir_subject(b->sarg, smt), format_lir_subject(b->targ, smt));    break;
        case LIR_iSUB: EMIT_COMMAND("sub %s, %s", format_lir_subject(b->sarg, smt), format_lir_subject(b->targ, smt));    break;
        case LIR_iMUL: EMIT_COMMAND("imul %s", format_lir_subject(b->sarg, smt));                                         break;
        case LIR_DIV:  EMIT_COMMAND("div %s", format_lir_subject(b->sarg, smt));                                          break;
        case LIR_iDIV: EMIT_COMMAND("idiv %s", format_lir_subject(b->sarg, smt));                                         break;
        case LIR_CMP:  EMIT_COMMAND("cmp %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));    break; // TODO: Fix translation
        case LIR_bAND:
        case LIR_iAND: EMIT_COMMAND("and %s, %s", format_lir_subject(b->sarg, smt), format_lir_subject(b->targ, smt));    break;
        case LIR_bOR:
        case LIR_iOR:  EMIT_COMMAND("or %s, %s", format_lir_subject(b->sarg, smt), format_lir_subject(b->targ, smt));     break;
        case LIR_fADD: EMIT_COMMAND("addsd %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));  break;
        case LIR_fSUB: EMIT_COMMAND("subsd %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));  break;
        case LIR_fMUL: EMIT_COMMAND("mulsd %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));  break;
        case LIR_fDIV: EMIT_COMMAND("divsd %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));  break;
        case LIR_fCMP: EMIT_COMMAND("ucomisd %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));break;
        case LIR_bXOR: EMIT_COMMAND("xor %s, %s", format_lir_subject(b->sarg, smt), format_lir_subject(b->targ, smt));    break;
        case LIR_bSHL: EMIT_COMMAND("shl %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));    break;
        case LIR_bSHR: EMIT_COMMAND("shr %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));    break;
        case LIR_bSAR: EMIT_COMMAND("sar %s, %s", format_lir_subject(b->farg, smt), format_lir_subject(b->sarg, smt));    break;
        case LIR_RAW: {
            string_t* raw_line = create_string(format_lir_subject(b->farg, smt));
            unsigned int percent_pos = raw_line->index_of(raw_line, '%');
            if (percent_pos < 0) EMIT_COMMAND("%s", raw_line->body);
            else {
                string_t* replacement = create_string(format_lir_subject(b->sarg, smt));
                string_t* line_str = create_string_from_part(raw_line->body, 0, percent_pos);
                line_str->cat(line_str, replacement);

                const char* suffix = line_str->body + 1;
                while (str_isdigit((unsigned char)*suffix)) {
                    suffix++;
                }

                string_t* suffix_str = create_string(suffix);
                line_str->cat(line_str, suffix_str);

                EMIT_COMMAND("%s", line_str->body);
                destroy_string(replacement);
                destroy_string(suffix_str);
                destroy_string(line_str);
            }

            destroy_string(raw_line);
            break;
        }
        default: break;
    }
}

static int _generate_ro_string(symbol_id_t id, sym_table_t* smt, FILE* output) {
    map_foreach (str_info_t* si, &smt->s.strtb) {
        if (si->t != STR_INDEPENDENT) continue;
        fprintf(output, "_str_%li_ db ", si->id);
        char* data = si->value->body;
        while (*data) {
            fprintf(output, "%i,", *(data++));
        }

        fprintf(output, "0\n");
    }

    return 1;
}

static int _generate_variable(symbol_id_t id, sym_table_t* smt, FILE* output) {
    variable_info_t vi;
    if (!VRTB_get_info_id(id, &vi, &smt->v)) {
        return;
    }

    if (
        vi.type == ARRAY_TYPE_TOKEN || 
        vi.type == STR_TYPE_TOKEN
    ) {
        array_info_t ai;
        if (!ARTB_get_info(vi.v_id, &ai, &smt->a)) return 0;
        token_t tmptkn = { .t_type = ai.elements_info.el_type, .flags = { .ptr = ai.elements_info.el_flags.ptr } };
        if (!list_size(&ai.elems)) {
            switch (TKN_variable_bitness(&tmptkn, 1)) {
                case TYPE_FULL_SIZE:    EMIT_COMMAND("%s resq %ld", vi.name->body, ai.size); break;
                case TYPE_HALF_SIZE:    EMIT_COMMAND("%s resd %ld", vi.name->body, ai.size); break;
                case TYPE_QUARTER_SIZE: EMIT_COMMAND("%s resw %ld", vi.name->body, ai.size); break;
                default:                EMIT_COMMAND("%s resb %ld", vi.name->body, ai.size); break;
            }
        }
        else {
            switch (TKN_variable_bitness(&tmptkn, 1)) {
                case TYPE_FULL_SIZE:    EMIT_COMMAND("%s dq ", vi.name->body); break;
                case TYPE_HALF_SIZE:    EMIT_COMMAND("%s dd ", vi.name->body); break;
                case TYPE_QUARTER_SIZE: EMIT_COMMAND("%s dw ", vi.name->body); break;
                default:                EMIT_COMMAND("%s db ", vi.name->body); break;
            }

            int elcount = ai.size;
            foreach (array_elem_info_t* el, &ai.elems) {
                fprintf(output, "%lu", el->value);
                if (elcount--) fprintf(output, ",");
            }

            if (elcount != ai.size && elcount) fprintf(output, ",");
            while (elcount-- > 0) {
                fprintf(output, "0");
                if (elcount) fprintf(output, ",");
            }

            fprintf(output, "\n");
        }

        return 1;
    }

    token_t tmptkn = { .t_type = vi.type, .flags = { .ptr = vi.vfs.ptr, .ro = vi.vfs.ro } };
    switch (TKN_variable_bitness(&tmptkn, 1)) {
        case TYPE_FULL_SIZE:    EMIT_COMMAND("%s dq 0\n", vi.name->body); break;
        case TYPE_HALF_SIZE:    EMIT_COMMAND("%s dd 0\n", vi.name->body); break;
        case TYPE_QUARTER_SIZE: EMIT_COMMAND("%s dw 0\n", vi.name->body); break;
        default:                EMIT_COMMAND("%s db 0\n", vi.name->body); break;
    }

    return 1;
}

static cfg_func_t* _find_function_by_id(symbol_id_t id, cfg_ctx_t* ctx) {
    foreach (cfg_func_t* fb, &ctx->funcs) {
        if (fb->fid == id) return fb;
    }

    return NULL;
}

int x86_64_generate_asm(cfg_ctx_t* cctx, sym_table_t* smt, FILE* output) {
    map_foreach (section_info_t* section, &smt->c.sectb) {
        EMIT_COMMAND("%s", section->name->body);
        foreach (symbol_id_t id, &section->vars) {
            _generate_variable(id, smt, output);
        }

        foreach (symbol_id_t id, &section->strs) {
            _generate_ro_string(id, smt, output);
        }

        foreach (symbol_id_t id, &section->func) {
            func_info_t fi;
            if (!FNTB_get_info_id(id, &fi, &smt->f)) continue;
            if (fi.flags.global)   EMIT_COMMAND("global %s", fi.name->body);
            if (fi.flags.external) EMIT_COMMAND("extern %s", fi.name->body);
            cfg_func_t* fb = _find_function_by_id(id, cctx);
            lir_block_t* lh = LIR_get_next(fb->lmap.entry, fb->lmap.exit, 0);
            while (lh) {
                _convert_lirblock_to_assembly(lh, &fi, smt, output);
                lh = LIR_get_next(lh, fb->lmap.exit, 1);
            }
        }
    }

    return 1;
}

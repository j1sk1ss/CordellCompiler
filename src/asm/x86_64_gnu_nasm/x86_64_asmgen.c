#include <asm/x86_64_asmgen.h>

int x86_64_generate_asm(lir_ctx_t* lctx, sym_table_t* smt, FILE* output) {
    x86_64_generate_data(smt, output);
    lir_block_t* curr = lctx->h;
    while (curr) {
        if (!curr->unused) switch (curr->op) {
            case LIR_FCLL:
            case LIR_ECLL: EMIT_COMMAND("call %s\n", format_lir_subject(curr->farg, smt)); break;

            case LIR_STRT:
            case LIR_FDCL: {
                EMIT_COMMAND("%s:\n", format_lir_subject(curr->farg, smt));
                EMIT_COMMAND("push rbp\n");
                EMIT_COMMAND("mov rbp, rsp\n");
                if (curr->sarg->storage.cnst.value > 0) {
                    EMIT_COMMAND("sub rsp, %ld\n", ALIGN(curr->sarg->storage.cnst.value, 8));
                }

                break;
            }

            case LIR_CDQ:  EMIT_COMMAND("cdq\n");     break;
            case LIR_SYSC: EMIT_COMMAND("syscall\n"); break;

            case LIR_FEND:
            case LIR_FRET: {
                EMIT_COMMAND("mov rsp, rbp\n");
                EMIT_COMMAND("pop rbp\n");
                EMIT_COMMAND("ret\n");
                break;
            }
            
            case LIR_FEXT: {
                func_info_t fi;
                if (FNTB_get_info_id(curr->farg->storage.cnst.value, &fi, &smt->f)) {
                    EMIT_COMMAND("extern %s\n", fi.name->body);
                }

                break;
            }

            case LIR_OEXT: {
                variable_info_t vi;
                if (VRTB_get_info_id(curr->farg->storage.cnst.value, &vi, &smt->v)) {
                    EMIT_COMMAND("extern %s\n", vi.name->body); 
                }

                break;
            }

            case LIR_BREAKPOINT: EMIT_COMMAND("int3 ; %s", format_lir_subject(curr->farg, smt));                                   break;
            case LIR_BB:         EMIT_COMMAND("\n; BB%ld:", curr->farg->storage.cnst.value);                                       break;

            case LIR_TST:  EMIT_COMMAND("test %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));  break;
            case LIR_XCHG: EMIT_COMMAND("xchg %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));  break;

            case LIR_MKLB: EMIT_COMMAND("%s:", format_lir_subject(curr->farg, smt));                                                break;
            
            case LIR_SETL: EMIT_COMMAND("setl %s", format_lir_subject(curr->farg, smt));                                            break;
            case LIR_SETG: EMIT_COMMAND("setg %s", format_lir_subject(curr->farg, smt));                                            break;
            case LIR_STLE: EMIT_COMMAND("setle %s", format_lir_subject(curr->farg, smt));                                           break;
            case LIR_STGE: EMIT_COMMAND("setge %s", format_lir_subject(curr->farg, smt));                                           break;
            case LIR_SETE: EMIT_COMMAND("sete %s", format_lir_subject(curr->farg, smt));                                            break;
            case LIR_STNE: EMIT_COMMAND("setne %s", format_lir_subject(curr->farg, smt));                                           break;
            case LIR_SETB: EMIT_COMMAND("setb %s", format_lir_subject(curr->farg, smt));                                            break;
            case LIR_SETA: EMIT_COMMAND("seta %s", format_lir_subject(curr->farg, smt));                                            break;
            case LIR_STBE: EMIT_COMMAND("setbe %s", format_lir_subject(curr->farg, smt));                                           break;
            case LIR_STAE: EMIT_COMMAND("setae %s", format_lir_subject(curr->farg, smt));                                           break;

            case LIR_NEG: EMIT_COMMAND("neg %s", format_lir_subject(curr->farg, smt));                                              break;
            case LIR_INC: EMIT_COMMAND("inc %s", format_lir_subject(curr->farg, smt));                                              break;
            case LIR_DEC: EMIT_COMMAND("dec %s", format_lir_subject(curr->farg, smt));                                              break;

            case LIR_JMP:  EMIT_COMMAND("jmp %s", format_lir_subject(curr->farg, smt));                                             break;
            case LIR_JE:   EMIT_COMMAND("je %s", format_lir_subject(curr->farg, smt));                                              break;
            case LIR_JLE:  EMIT_COMMAND("jle %s", format_lir_subject(curr->farg, smt));                                             break;
            case LIR_JNE:  EMIT_COMMAND("jne %s", format_lir_subject(curr->farg, smt));                                             break;
            case LIR_JL:   EMIT_COMMAND("jl %s", format_lir_subject(curr->farg, smt));                                              break;
            case LIR_JG:   EMIT_COMMAND("jg %s", format_lir_subject(curr->farg, smt));                                              break;
            case LIR_JGE:  EMIT_COMMAND("jge %s", format_lir_subject(curr->farg, smt));                                             break;
            case LIR_JA:   EMIT_COMMAND("ja %s", format_lir_subject(curr->farg, smt));                                              break;
            case LIR_JAE:  EMIT_COMMAND("jae %s", format_lir_subject(curr->farg, smt));                                             break;
            case LIR_JB:   EMIT_COMMAND("jb %s", format_lir_subject(curr->farg, smt));                                              break;
            case LIR_JBE:  EMIT_COMMAND("jbe %s", format_lir_subject(curr->farg, smt));                                             break;

            case LIR_aMOV:
            case LIR_iMOV:  EMIT_COMMAND("mov %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));   break;
            case LIR_MOVZX: EMIT_COMMAND("movzx %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt)); break;
            case LIR_MOVSX: EMIT_COMMAND("movsx %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt)); break;

            case LIR_fMOV:
            case LIR_fMVf:  EMIT_COMMAND("movsd %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt)); break;
            case LIR_REF:   EMIT_COMMAND("lea %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));   break;
            case LIR_LDREF: EMIT_COMMAND("mov [%s], %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt)); break;
            case LIR_GDREF: EMIT_COMMAND("mov %s, [%s]", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt)); break;

            case LIR_PUSH: EMIT_COMMAND("push %s", format_lir_subject(curr->farg, smt));                                            break;
            case LIR_POP:  EMIT_COMMAND("pop %s", format_lir_subject(curr->farg, smt));                                             break;
            
            case LIR_iADD: EMIT_COMMAND("add %s, %s", format_lir_subject(curr->sarg, smt), format_lir_subject(curr->targ, smt));    break;
            case LIR_iSUB: EMIT_COMMAND("sub %s, %s", format_lir_subject(curr->sarg, smt), format_lir_subject(curr->targ, smt));    break;
            case LIR_iMUL: EMIT_COMMAND("imul %s", format_lir_subject(curr->sarg, smt));                                            break;
            case LIR_DIV:  EMIT_COMMAND("div %s", format_lir_subject(curr->sarg, smt));                                             break;
            case LIR_iDIV: EMIT_COMMAND("idiv %s", format_lir_subject(curr->sarg, smt));                                            break;
            case LIR_CMP:  EMIT_COMMAND("cmp %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));    break; // TODO: Fix translation

            case LIR_bAND:
            case LIR_iAND: EMIT_COMMAND("and %s, %s", format_lir_subject(curr->sarg, smt), format_lir_subject(curr->targ, smt));    break;
            case LIR_bOR:
            case LIR_iOR:  EMIT_COMMAND("or %s, %s", format_lir_subject(curr->sarg, smt), format_lir_subject(curr->targ, smt));     break;

            case LIR_fADD: EMIT_COMMAND("addsd %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));  break;
            case LIR_fSUB: EMIT_COMMAND("subsd %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));  break;
            case LIR_fMUL: EMIT_COMMAND("mulsd %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));  break;
            case LIR_fDIV: EMIT_COMMAND("divsd %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));  break;
            case LIR_fCMP: EMIT_COMMAND("ucomisd %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));break;

            case LIR_bXOR: EMIT_COMMAND("xor %s, %s", format_lir_subject(curr->sarg, smt), format_lir_subject(curr->targ, smt));    break;
            case LIR_bSHL: EMIT_COMMAND("shl %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));    break;
            case LIR_bSHR: EMIT_COMMAND("shr %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));    break;
            case LIR_bSAR: EMIT_COMMAND("sar %s, %s", format_lir_subject(curr->farg, smt), format_lir_subject(curr->sarg, smt));    break;

            case LIR_RAW: {
                string_t* raw_line = create_string(format_lir_subject(curr->farg, smt));
                unsigned int percent_pos = raw_line->index_of(raw_line, '%');
                if (percent_pos < 0) EMIT_COMMAND("%s", raw_line->body);
                else {
                    string_t* replacement = create_string(format_lir_subject(curr->sarg, smt));
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

            case LIR_STEND:
            case LIR_EXITOP: {
                EMIT_COMMAND("mov rax, 0x2000001");
                EMIT_COMMAND("syscall");
                break;
            }

            default: break;
        }

        curr = curr->next;
    }

    return 1;
}

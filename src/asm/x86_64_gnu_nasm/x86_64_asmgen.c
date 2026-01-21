#include <asm/x86_64_asmgen.h>

int x86_64_generate_asm(lir_ctx_t* lctx, sym_table_t* smt, FILE* output) {
    x86_64_generate_data(smt, output);
    lir_block_t* curr = lctx->h;
    while (curr) {
        if (!curr->unused) switch (curr->op) {
            case LIR_FCLL:
            case LIR_ECLL: fprintf(output, "call %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            
            case LIR_FEND: {
                x86_64_kill_stackframe(output);
                fprintf(output, "ret\n");
                break;
            }

            case LIR_STRT: {
                func_info_t fi;
                if (FNTB_get_info_id(curr->farg->storage.str.sid, &fi, &smt->f)) {
                    fprintf(output, "%s:\n", fi.name->body);
                    x86_64_generate_stackframe(curr, LIR_STEND, output);
                }

                break;
            }

            case LIR_FDCL: {
                fprintf(output, "%s:\n", x86_64_asm_variable(curr->farg, smt));
                x86_64_generate_stackframe(curr, LIR_FEND, output);
                break;
            }

            case LIR_CDQ:  fprintf(output, "cdq\n");     break;
            case LIR_SYSC: fprintf(output, "syscall\n"); break;
            case LIR_FRET: {
                x86_64_kill_stackframe(output);
                fprintf(output, "ret\n");
                break;
            }
            
            case LIR_FEXT: {
                func_info_t fi;
                if (FNTB_get_info_id(curr->farg->storage.cnst.value, &fi, &smt->f)) {
                    fprintf(output, "extern %s\n", fi.name->body);
                }

                break;
            }

            case LIR_OEXT: {
                variable_info_t vi;
                if (VRTB_get_info_id(curr->farg->storage.cnst.value, &vi, &smt->v)) {
                    fprintf(output, "extern %s\n", vi.name->body); 
                }

                break;
            }

            case LIR_BREAKPOINT: fprintf(output, "int3 ; %s\n", x86_64_asm_variable(curr->farg, smt));                                      break;
            case LIR_BB:         fprintf(output, "\n; BB%ld: \n", curr->farg->storage.cnst.value);                                          break;

            case LIR_TST:  fprintf(output, "test %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));    break;
            case LIR_XCHG: fprintf(output, "xchg %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));    break;

            case LIR_MKLB: fprintf(output, "%s:\n", x86_64_asm_variable(curr->farg, smt));                                                  break;
            
            case LIR_SETL: fprintf(output, "setl %s\n", x86_64_asm_variable(curr->farg, smt));                                              break;
            case LIR_SETG: fprintf(output, "setg %s\n", x86_64_asm_variable(curr->farg, smt));                                              break;
            case LIR_STLE: fprintf(output, "setle %s\n", x86_64_asm_variable(curr->farg, smt));                                             break;
            case LIR_STGE: fprintf(output, "setge %s\n", x86_64_asm_variable(curr->farg, smt));                                             break;
            case LIR_SETE: fprintf(output, "sete %s\n", x86_64_asm_variable(curr->farg, smt));                                              break;
            case LIR_STNE: fprintf(output, "setne %s\n", x86_64_asm_variable(curr->farg, smt));                                             break;
            case LIR_SETB: fprintf(output, "setb %s\n", x86_64_asm_variable(curr->farg, smt));                                              break;
            case LIR_SETA: fprintf(output, "seta %s\n", x86_64_asm_variable(curr->farg, smt));                                              break;
            case LIR_STBE: fprintf(output, "setbe %s\n", x86_64_asm_variable(curr->farg, smt));                                             break;
            case LIR_STAE: fprintf(output, "setae %s\n", x86_64_asm_variable(curr->farg, smt));                                             break;

            case LIR_NEG: fprintf(output, "neg %s\n", x86_64_asm_variable(curr->farg, smt));                                                break;
            case LIR_INC: fprintf(output, "inc %s\n", x86_64_asm_variable(curr->farg, smt));                                                break;
            case LIR_DEC: fprintf(output, "dec %s\n", x86_64_asm_variable(curr->farg, smt));                                                break;

            case LIR_JMP:  fprintf(output, "jmp %s\n", x86_64_asm_variable(curr->farg, smt));                                               break;
            case LIR_JE:   fprintf(output, "je %s\n", x86_64_asm_variable(curr->farg, smt));                                                break;
            case LIR_JLE:  fprintf(output, "jle %s\n", x86_64_asm_variable(curr->farg, smt));                                               break;
            case LIR_JNE:  fprintf(output, "jne %s\n", x86_64_asm_variable(curr->farg, smt));                                               break;
            case LIR_JL:   fprintf(output, "jl %s\n", x86_64_asm_variable(curr->farg, smt));                                                break;
            case LIR_JG:   fprintf(output, "jg %s\n", x86_64_asm_variable(curr->farg, smt));                                                break;
            case LIR_JGE:  fprintf(output, "jge %s\n", x86_64_asm_variable(curr->farg, smt));                                               break;
            case LIR_JA:   fprintf(output, "ja %s\n", x86_64_asm_variable(curr->farg, smt));                                                break;
            case LIR_JAE:  fprintf(output, "jae %s\n", x86_64_asm_variable(curr->farg, smt));                                               break;
            case LIR_JB:   fprintf(output, "jb %s\n", x86_64_asm_variable(curr->farg, smt));                                                break;
            case LIR_JBE:  fprintf(output, "jbe %s\n", x86_64_asm_variable(curr->farg, smt));                                               break;

            case LIR_aMOV:
            case LIR_iMOV:  fprintf(output, "mov %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));    break;
            case LIR_MOVZX: fprintf(output, "movzx %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));  break;
            case LIR_MOVSX: fprintf(output, "movsx %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));  break;

            case LIR_fMOV:
            case LIR_fMVf:  fprintf(output, "movsd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));  break;
            case LIR_REF:   fprintf(output, "lea %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));    break;
            case LIR_LDREF: fprintf(output, "mov [%s], %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));  break;
            case LIR_GDREF: fprintf(output, "mov %s, [%s]\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));  break;

            case LIR_PUSH: fprintf(output, "push %s\n", x86_64_asm_variable(curr->farg, smt));                                              break;
            case LIR_POP:  fprintf(output, "pop %s\n", x86_64_asm_variable(curr->farg, smt));                                               break;
            
            case LIR_iADD: fprintf(output, "add %s, %s\n", x86_64_asm_variable(curr->sarg, smt), x86_64_asm_variable(curr->targ, smt));     break;
            case LIR_iSUB: fprintf(output, "sub %s, %s\n", x86_64_asm_variable(curr->sarg, smt), x86_64_asm_variable(curr->targ, smt));     break;
            case LIR_iMUL: fprintf(output, "imul %s\n", x86_64_asm_variable(curr->sarg, smt));                                              break;
            case LIR_DIV:  fprintf(output, "div %s\n", x86_64_asm_variable(curr->sarg, smt));                                               break;
            case LIR_iDIV: fprintf(output, "idiv %s\n", x86_64_asm_variable(curr->sarg, smt));                                              break;
            case LIR_CMP:  fprintf(output, "cmp %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));     break;

            case LIR_bAND:
            case LIR_iAND: fprintf(output, "and %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));     break;
            case LIR_bOR:
            case LIR_iOR:  fprintf(output, "or %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));      break;

            case LIR_fADD: fprintf(output, "addsd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));   break;
            case LIR_fSUB: fprintf(output, "subsd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));   break;
            case LIR_fMUL: fprintf(output, "mulsd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));   break;
            case LIR_fDIV: fprintf(output, "divsd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));   break;
            case LIR_fCMP: fprintf(output, "ucomisd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;

            case LIR_bXOR: fprintf(output, "xor %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));     break;
            case LIR_bSHL: fprintf(output, "shl %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));     break;
            case LIR_bSHR: fprintf(output, "shr %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));     break;
            case LIR_bSAR: fprintf(output, "sar %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));     break;

            case LIR_RAW: {
                string_t* raw_line = create_string(x86_64_asm_variable(curr->farg, smt));
                unsigned int percent_pos = raw_line->index_of(raw_line, '%');
                if (percent_pos < 0) fprintf(output, "%s\n", raw_line->body);
                else {
                    string_t* replacement = create_string(x86_64_asm_variable(curr->sarg, smt));
                    string_t* line_str = create_string_from_part(raw_line->body, 0, percent_pos);
                    line_str->cat(line_str, replacement);

                    const char* suffix = line_str->body + 1;
                    while (str_isdigit((unsigned char)*suffix)) {
                        suffix++;
                    }

                    string_t* suffix_str = create_string(suffix);
                    line_str->cat(line_str, suffix_str);

                    fprintf(output, "%s\n", line_str->body);
                    destroy_string(replacement);
                    destroy_string(suffix_str);
                    destroy_string(line_str);
                }

                destroy_string(raw_line);
                break;
            }

            case LIR_EXITOP: {
                fprintf(output, "mov rax, 0x2000001\n");
                fprintf(output, "syscall\n");
                break;
            }

            default: break;
        }

        curr = curr->next;
    }

    return 1;
}

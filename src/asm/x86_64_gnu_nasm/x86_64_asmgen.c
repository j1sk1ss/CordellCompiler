#include <asm/x86_64_gnu_nasm/x86_64_asmgen.h>

int x86_64_generate_asm(lir_ctx_t* lctx, sym_table_t* smt, FILE* output) {
    x86_64_generate_data(smt, output);
    lir_block_t* curr = lctx->h;
    while (curr) {
        switch (curr->op) {
            case LIR_FCLL:
            case LIR_ECLL: fprintf(output, "call %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            
            case LIR_STRT: {
                fprintf(output, "global _start\n");
                fprintf(output, "_start:\n");
                x86_64_generate_stackframe(curr, LIR_STEND, output);
                break;
            }

            case LIR_FDCL: {
                fprintf(output, "%s:\n", x86_64_asm_variable(curr->farg, smt));
                x86_64_generate_stackframe(curr, LIR_FEND, output);
                break;
            }

            case LIR_CDQ:  fprintf(output, "cdq\n");    break;
            case LIR_SYSC: fprintf(output, "syscall\n"); break;
            case LIR_FRET: {
                fprintf(output, "mov rax, %s\n", x86_64_asm_variable(curr->farg, smt));
                x86_64_kill_stackframe(output);
                fprintf(output, "ret\n");
                break;
            }
            
            case LIR_TDBL: fprintf(output, "cvtsi2sd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;
            case LIR_TST: fprintf(output, "test %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));      break;
            case LIR_XCHG: fprintf(output, "xchg %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));     break;

            case LIR_MKLB: fprintf(output, "%s:\n", x86_64_asm_variable(curr->farg, smt));       break;
            case LIR_OEXT: fprintf(output, "extern %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            
            case LIR_SETL: fprintf(output, "setl %s\n", x86_64_asm_variable(curr->farg, smt));  break;
            case LIR_SETG: fprintf(output, "setg %s\n", x86_64_asm_variable(curr->farg, smt));  break;
            case LIR_STLE: fprintf(output, "setle %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            case LIR_STGE: fprintf(output, "setge %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            case LIR_SETE: fprintf(output, "sete %s\n", x86_64_asm_variable(curr->farg, smt));  break;
            case LIR_STNE: fprintf(output, "setne %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            case LIR_SETB: fprintf(output, "setb %s\n", x86_64_asm_variable(curr->farg, smt));  break;
            case LIR_SETA: fprintf(output, "seta %s\n", x86_64_asm_variable(curr->farg, smt));  break;
            case LIR_STBE: fprintf(output, "setbe %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            case LIR_STAE: fprintf(output, "setae %s\n", x86_64_asm_variable(curr->farg, smt)); break;

            case LIR_JMP: fprintf(output, "jmp %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            case LIR_JE:  fprintf(output, "je %s\n", x86_64_asm_variable(curr->farg, smt));  break;
            case LIR_JLE: fprintf(output, "jle %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            case LIR_JNE: fprintf(output, "jne %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            case LIR_JL:  fprintf(output, "jl %s\n", x86_64_asm_variable(curr->farg, smt));  break;
            case LIR_JG:  fprintf(output, "jg %s\n", x86_64_asm_variable(curr->farg, smt));  break;
            case LIR_JGE: fprintf(output, "jge %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            case LIR_JA:  fprintf(output, "ja %s\n", x86_64_asm_variable(curr->farg, smt));  break;
            case LIR_JAE: fprintf(output, "jae %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            case LIR_JB:  fprintf(output, "jb %s\n", x86_64_asm_variable(curr->farg, smt));  break;
            case LIR_JBE: fprintf(output, "jbe %s\n", x86_64_asm_variable(curr->farg, smt)); break;

            case LIR_iMOVq:
            case LIR_iMOV:  fprintf(output, "mov %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));   break;
            case LIR_iMVZX: fprintf(output, "movzx %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;
            case LIR_iMVSX: fprintf(output, "movsx %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;

            case LIR_fMOV:
            case LIR_fMVf:  fprintf(output, "movsd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;
            case LIR_REF:   fprintf(output, "lea %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));   break;
            case LIR_GDREF: fprintf(output, "mov %s, [%s]\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;
            case LIR_LDREF: fprintf(output, "mov [%s], %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;

            case LIR_PUSH: fprintf(output, "push %s\n", x86_64_asm_variable(curr->farg, smt)); break;
            case LIR_POP:  fprintf(output, "pop %s\n", x86_64_asm_variable(curr->farg, smt));  break;
            
            case LIR_iADD: fprintf(output, "add %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));  break;
            case LIR_iSUB: fprintf(output, "sub %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));  break;
            case LIR_iMUL: fprintf(output, "imul %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;
            case LIR_DIV:  fprintf(output, "div %s\n", x86_64_asm_variable(curr->farg, smt));                                            break;
            case LIR_iDIV: fprintf(output, "idiv %s\n", x86_64_asm_variable(curr->farg, smt));                                           break;
            case LIR_iCMP: fprintf(output, "cmp %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));  break;

            case LIR_bAND:
            case LIR_iAND: fprintf(output, "and %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;
            case LIR_bOR:
            case LIR_iOR:  fprintf(output, "or %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));  break;

            case LIR_fADD: fprintf(output, "addsd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));   break;
            case LIR_fSUB: fprintf(output, "subsd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));   break;
            case LIR_fMUL: fprintf(output, "mulsd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));   break;
            case LIR_fDIV: fprintf(output, "divsd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt));   break;
            case LIR_fCMP: fprintf(output, "ucomisd %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;

            case LIR_bXOR: fprintf(output, "xor %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;
            case LIR_bSHL: fprintf(output, "shl %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;
            case LIR_bSHR: fprintf(output, "shr %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;
            case LIR_bSAR: fprintf(output, "sar %s, %s\n", x86_64_asm_variable(curr->farg, smt), x86_64_asm_variable(curr->sarg, smt)); break;

            case LIR_RAW:  fprintf(output, "%s\n", x86_64_asm_variable(curr->farg, smt)); break;
            case LIR_EXITOP: {
                fprintf(output, "mov rax, 60\n");
                fprintf(output, "mov rdi, %s\n", x86_64_asm_variable(curr->farg, smt));
                fprintf(output, "syscall\n");
                break;
            }

            default: fprintf(output, "; op=%d\n", curr->op); break;
        }

        curr = curr->next;
    }

    return 1;
}

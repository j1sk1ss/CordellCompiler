#include <asm/x86_64_gnu_nasm/x86_64_asmgen.h>

int x86_64_generate_asm(ir_block_t* h, FILE* output) {
    ir_block_t* curr = h;
    while (curr) {
        switch (curr->op) {
            case FCLL: iprintf(output, "call _cpl_%s\n", GET_X86_64_IRVAR(curr->farg)); break;
            case ECLL: iprintf(output, "call %s\n", GET_X86_64_IRVAR(curr->farg));      break;
            case STRT: {
                iprintf(output, "global _start\n");
                iprintf(output, "_start:\n");
                break;
            }
            case SYSC: iprintf(output, "syscall\n"); break;
            case FRET: iprintf(output, "ret\n");     break;
            case TDBL:
                iprintf(output, "cvtsi2sd %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg));
            break;
            case TST:
                iprintf(output, "test %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg));
            break;
            case XCHG:
                iprintf(output, "xchg %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg));
            break;
            case CDQ: iprintf(output, "cdq\n");                               break;
            case MKLB: iprintf(output, "%s:\n", GET_X86_64_IRVAR(curr->farg));       break;
            case FDCL: iprintf(output, "_cpl_%s:\n", GET_X86_64_IRVAR(curr->farg));  break;
            case OEXT: iprintf(output, "extern %s\n", GET_X86_64_IRVAR(curr->farg)); break;
            
            case SETL: iprintf(output, "setl %s\n", GET_X86_64_IRVAR(curr->farg));  break;
            case SETG: iprintf(output, "setg %s\n", GET_X86_64_IRVAR(curr->farg));  break;
            case STLE: iprintf(output, "setle %s\n", GET_X86_64_IRVAR(curr->farg)); break;
            case STGE: iprintf(output, "setge %s\n", GET_X86_64_IRVAR(curr->farg)); break;
            case SETE: iprintf(output, "sete %s\n", GET_X86_64_IRVAR(curr->farg));  break;
            case STNE: iprintf(output, "setne %s\n", GET_X86_64_IRVAR(curr->farg)); break;
            case SETB: iprintf(output, "setb %s\n", GET_X86_64_IRVAR(curr->farg));  break;
            case SETA: iprintf(output, "seta %s\n", GET_X86_64_IRVAR(curr->farg));  break;
            case STBE: iprintf(output, "setbe %s\n", GET_X86_64_IRVAR(curr->farg)); break;
            case STAE: iprintf(output, "setae %s\n", GET_X86_64_IRVAR(curr->farg)); break;

            case JMP: iprintf(output, "jmp %s\n", GET_X86_64_IRVAR(curr->farg)); break;
            case JE:  iprintf(output, "je %s\n", GET_X86_64_IRVAR(curr->farg));  break;
            case JNE: iprintf(output, "jne %s\n", GET_X86_64_IRVAR(curr->farg)); break;
            case JL:  iprintf(output, "jl %s\n", GET_X86_64_IRVAR(curr->farg));  break;
            case JG:  iprintf(output, "jg %s\n", GET_X86_64_IRVAR(curr->farg));  break;

            case iMOVq: iprintf(output, "movq %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg));  break;
            case iMOV:  iprintf(output, "mov %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg));   break;
            case iMVZX: iprintf(output, "movzx %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg)); break;

            case fMOV:
            case fMVf: iprintf(output, "movsd %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg)); break;
            case LEA: 
                iprintf(output, "lea %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg)); 
            break;
            case PUSH: iprintf(output, "push %s\n", GET_X86_64_IRVAR(curr->farg)); break;
            case POP: iprintf(output, "pop %s\n", GET_X86_64_IRVAR(curr->farg));   break;
            
            case iADD: iprintf(output, "add %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg));   break;
            case iSUB: iprintf(output, "sub %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg));   break;
            case iMUL: iprintf(output, "imul %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg));  break;
            case DIV:  iprintf(output, "div %s\n", GET_X86_64_IRVAR(curr->farg));                              break;
            case iDIV: 
            case iMOD: iprintf(output, "idiv %s\n", GET_X86_64_IRVAR(curr->farg)); break;
            case iLRG:
            case iLGE:
            case iLWR:
            case iLRE:
            case iCMP: iprintf(output, "cmp %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg)); break;

            case bAND:
            case iAND: iprintf(output, "and %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg)); break;
            case bOR:
            case iOR:  iprintf(output, "or %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg)); break;

            case fADD: iprintf(output, "addsd %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg));   break;
            case fSUB: iprintf(output, "subsd %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg));   break;
            case fMUL: iprintf(output, "mulsd %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg));   break;
            case fDIV: iprintf(output, "divsd %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg));   break;
            case fCMP: iprintf(output, "ucomisd %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg)); break;

            case bXOR: iprintf(output, "xor %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg)); break;
            case bSHL: iprintf(output, "shl %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg)); break;
            case bSHR: iprintf(output, "shr %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg)); break;
            case bSAR: iprintf(output, "sar %s, %s\n", GET_X86_64_IRVAR(curr->farg), GET_X86_64_IRVAR(curr->sarg)); break;

            case RAW: iprintf(output, "%s\n", GET_X86_64_IRVAR(curr->farg)); break;
            default:
                iprintf(output, "; unknown op %d\n", curr->op);
            break;
        }

        curr = curr->next;
    }

    return 1;
}

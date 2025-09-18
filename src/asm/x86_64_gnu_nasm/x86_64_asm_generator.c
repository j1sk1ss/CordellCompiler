#include <asm/x86_64_gnu_nasm/x86_64_asm_generator.h>

int x86_64_generate_asm(ir_block_t* h, FILE* output) {
    ir_block_t* curr = h;
    while (curr) {
        switch (curr->op) {
            case FCLL: iprintf(output, "call _cpl_%s\n", GET_IRVAR(curr->farg)); break;
            case ECLL: iprintf(output, "call %s\n", GET_IRVAR(curr->farg));      break;
            case STRT: {
                iprintf(output, "global _start\n");
                iprintf(output, "_start:\n");
                break;
            }
            case SYSC: iprintf(output, "syscall\n"); break;
            case FRET: iprintf(output, "ret\n");     break;
            case TDBL: break;
            case TST: 
                iprintf(output, "tst %s, %s", GET_IRVAR(curr->farg), GET_IRVAR(curr->sarg)); 
            break;
            case XCHG: break;
            case CDQ: iprintf(output, "cdq\n");                               break;
            case MKLB: iprintf(output, "%s:\n", GET_IRVAR(curr->farg));       break;
            case FDCL: iprintf(output, "_cpl_%s:\n", GET_IRVAR(curr->farg));  break;
            case OEXT: iprintf(output, "extern %s\n", GET_IRVAR(curr->farg)); break;
            
            case SETL: iprintf(output, "setl %s\n", GET_IRVAR(curr->farg));  break;
            case SETG: iprintf(output, "setg %s\n", GET_IRVAR(curr->farg));  break;
            case STLE: iprintf(output, "setle %s\n", GET_IRVAR(curr->farg)); break;
            case STGE: iprintf(output, "setge %s\n", GET_IRVAR(curr->farg)); break;
            case SETE: iprintf(output, "sete %s\n", GET_IRVAR(curr->farg));  break;
            case STNE: iprintf(output, "setne %s\n", GET_IRVAR(curr->farg)); break;
            case SETB: iprintf(output, "setb %s\n", GET_IRVAR(curr->farg));  break;
            case SETA: iprintf(output, "seta %s\n", GET_IRVAR(curr->farg));  break;
            case STBE: iprintf(output, "setbe %s\n", GET_IRVAR(curr->farg)); break;
            case STAE: iprintf(output, "setae %s\n", GET_IRVAR(curr->farg)); break;

            case JMP: iprintf(output, "jmp %s\n", GET_IRVAR(curr->farg)); break;
            case JE:  iprintf(output, "je %s\n", GET_IRVAR(curr->farg));  break;
            case JNE: iprintf(output, "jne %s\n", GET_IRVAR(curr->farg)); break;
            case JL:  iprintf(output, "jl %s\n", GET_IRVAR(curr->farg));  break;
            case JG:  iprintf(output, "jg %s\n", GET_IRVAR(curr->farg));  break;

            case iMOV:  iprintf(output, "mov %s, %s\n", GET_IRVAR(curr->farg), GET_IRVAR(curr->sarg));       break;
            case iMOVb: iprintf(output, "mov byte %s, %s\n", GET_IRVAR(curr->farg), GET_IRVAR(curr->sarg));  break;
            case iMOVw: iprintf(output, "mov word %s, %s\n", GET_IRVAR(curr->farg), GET_IRVAR(curr->sarg));  break;
            case iMOVd: iprintf(output, "mov dword %s, %s\n", GET_IRVAR(curr->farg), GET_IRVAR(curr->sarg)); break;
            case iMOVq: iprintf(output, "mov qword %s, %s\n", GET_IRVAR(curr->farg), GET_IRVAR(curr->sarg)); break;
            case iMVZX: iprintf(output, "movzx %s, %s\n", GET_IRVAR(curr->farg), GET_IRVAR(curr->sarg));     break;

            case fMOV: break;
            case fMVf: break;
            case LEA: 
                iprintf(output, "lea %s, %s\n", GET_IRVAR(curr->farg), GET_IRVAR(curr->sarg)); 
            break;
            case PUSH: iprintf(output, "push %s\n", GET_IRVAR(curr->sarg)); break;
            case POP: iprintf(output, "pop %s\n", GET_IRVAR(curr->sarg));   break;
            
            case iADD: break;
            case iSUB: break;
            case iMUL: break;
            case DIV: break;
            case iDIV: break;
            case iMOD: break;
            case iLRG: break;
            case iLGE: break;
            case iLWR: break;
            case iLRE: break;
            case iCMP: break;

            case iAND: break;
            case iOR: break;

            case fADD: break;
            case fSUB: break;
            case fMUL: break;
            case fDIV: break;
            case fCMP: break;

            case bAND: break;
            case bOR: break;
            case bXOR: break;
            case bSHL: break;
            case bSHR: break;
            case bSAR: break;

            case RAW: iprintf("%s\n", GET_IRVAR(curr->farg)); break;
            default: break;
        }

        curr = curr->next;
    }

    return 1;
}

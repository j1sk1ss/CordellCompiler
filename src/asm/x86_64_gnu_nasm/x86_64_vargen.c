#include <asm/x86_64_gnu_nasm/x86_64_asmgen.h>

static char _buffers[2][128];
static int _idx = 0;

const char* x86_64_asm_variable(lir_subject_t* v, sym_table_t* smt) {
    char* curr_buffer = _buffers[_idx];
    _idx = (_idx + 1) % 2;

    switch (v->t) {
        case LIR_NUMBER: return v->storage.num.value;
        case LIR_CONSTVAL: {
            snprintf(curr_buffer, 128, "%d", v->storage.cnst.value);
            return curr_buffer;
        }

        case LIR_MEMORY: {
            if (v->storage.var.offset > 0) snprintf(curr_buffer, 128, "[rbp - %d]", v->storage.var.offset);
            else  snprintf(curr_buffer, 128, "[rbp + %d]", ABS(v->storage.var.offset));
            return curr_buffer;
        }
        
        case LIR_GLVARIABLE: {
            variable_info_t vi;
            if (VRTB_get_info_id(v->storage.var.v_id, &vi, &smt->v)) {
                snprintf(curr_buffer, 128, "[rel %s]", vi.name);
                return curr_buffer;
            }

            break;
        }

        case LIR_LABEL: {
            snprintf(curr_buffer, 128, "lb%i", v->storage.lb.lb_id);
            return curr_buffer;
        }

        case LIR_STRING: {
            snprintf(curr_buffer, 128, "[rel _str_%d_]", v->storage.str.sid);
            return curr_buffer;
        }

        case LIR_FNAME: {
            func_info_t fi;
            if (FNTB_get_info_id(v->storage.str.sid, &fi, &smt->f)) {
                if (fi.global) snprintf(curr_buffer, 128, "%s", fi.name);
                else snprintf(curr_buffer, 128, "_cpl_%s", fi.name);
                return curr_buffer;
            }

            break;
        }

        case LIR_RAWASM: {
            str_info_t si;
            if (STTB_get_info_id(v->storage.str.sid, &si, &smt->s)) {
                snprintf(curr_buffer, 128, "%s", si.value);
                return curr_buffer;
            }
        }

        case LIR_REGISTER: {
            switch (v->storage.reg.reg) {
                case XMM0: return "xmm0";
                case XMM1: return "xmm1";
                case XMM2: return "xmm2";
                case XMM3: return "xmm3";
                case XMM4: return "xmm4";
                case RAX:  return "rax";
                case RBX:  return "rbx";
                case RCX:  return "rcx";
                case RDX:  return "rdx";
                case RSI:  return "rsi";
                case RDI:  return "rdi";
                case RBP:  return "rbp";
                case RSP:  return "rsp";

                case R8:   return "r8";
                case R9:   return "r9";
                case R10:  return "r10";
                case R11:  return "r11";
                case R12:  return "r12";
                case R13:  return "r13";
                case R14:  return "r14";
                case R15:  return "r15";

                case R8D:  return "r8d";
                case R9D:  return "r9d";
                case R10D: return "r10d";
                case R11D: return "r11d";
                case R12D: return "r12d";
                case R13D: return "r13d";
                case R14D: return "r14d";
                case R15D: return "r15d";

                case R8W:  return "r8w";
                case R9W:  return "r9w";
                case R10W: return "r10w";
                case R11W: return "r11w";
                case R12W: return "r12w";
                case R13W: return "r13w";
                case R14W: return "r14w";
                case R15W: return "r15w";

                case R8B:  return "r8b";
                case R9B:  return "r9b";
                case R10B: return "r10b";
                case R11B: return "r11b";
                case R12B: return "r12b";
                case R13B: return "r13b";
                case R14B: return "r14b";
                case R15B: return "r15b";

                case EAX:  return "eax";
                case EBX:  return "ebx";
                case ECX:  return "ecx";
                case EDX:  return "edx";
                case ESI:  return "esi";
                case EDI:  return "edi";
                case EBP:  return "ebp";
                case ESP:  return "esp";

                case AX:   return "ax";
                case BX:   return "bx";
                case CX:   return "cx";
                case DX:   return "dx";
                case SI:   return "si";
                case DI:   return "di";
                case BP:   return "bp";
                case SP:   return "sp";

                case AL:   return "al";
                case BL:   return "bl";
                case CL:   return "cl";
                case DL:   return "dl";
                case SIL:  return "sil";
                case DIL:  return "dil";
                case BPL:  return "bpl";
                case SPL:  return "spl";

                case AH:   return "ah";
                case BH:   return "bh";
                case CH:   return "ch";
                case DH:   return "dh";
            }
        }

        default: break;
    }

    return curr_buffer;
}

#include <asm/x86_64_asmgen.h>

/* Idea is to prevent the same buffer usage in the output code.
   To do this, the one approach is to use two buffers. */
static char _buffers[2][136];
static int _idx = 0;

/* Get the next buffer in the sequence.
   - Returns a pointer to the current buffer. */
static inline char* _get_buffer() {
    char* curr_buffer = _buffers[_idx];
    _idx = (_idx + 1) % 2;
    return curr_buffer;
}

const char* format_lir_subject(lir_subject_t* v, sym_table_t* smt) {
    char* buffer = _get_buffer();
    switch (v->t) {
        case LIR_NUMBER: return v->storage.num.value->body;
        case LIR_CONSTVAL: {
            snprintf(buffer, sizeof(_buffers[0]), "%ld", v->storage.cnst.value);
            return buffer;
        }
        case LIR_LABEL: {
            snprintf(buffer, sizeof(_buffers[0]), "lb%li", v->storage.lb.lb_id);
            return buffer;
        }
        case LIR_RAWASM:
        case LIR_STRING: {
            str_info_t si;
            if (STTB_get_info_id(v->storage.str.sid, &si, &smt->s)) {
                switch (si.t) {
                    case STR_INDEPENDENT: {
                        snprintf(buffer, sizeof(_buffers[0]), "[rel _str_%ld_]", v->storage.str.sid); 
                        return buffer;
                    }
                    case STR_RAW_ASM:
                    case STR_COMMENT: return si.value->body;
                    default: break;
                }
            }

            return "<unknown>";
        }
        case LIR_FNAME: {
            func_info_t fi;
            if (FNTB_get_info_id(v->storage.str.sid, &fi, &smt->f)) {
                char *local = "_cpl_%s", *global = "%s";
                if (v->storage.str.rel) {
                    local = "[_cpl_%s]";
                    global = "[%s]";
                }

                if (
                    fi.flags.global || fi.flags.external
                ) snprintf(buffer, sizeof(_buffers[0]), global, fi.name->body);
                else snprintf(buffer, sizeof(_buffers[0]), local, fi.virt->body);
                return buffer;
            }

            return "<unknown>";
        }
        case LIR_GLVARIABLE:
        case LIR_VARIABLE: {
            variable_info_t vi;
            if (VRTB_get_info_id(v->storage.var.v_id, &vi, &smt->v)) {
                if (vi.vfs.glob) {
                    snprintf(buffer, sizeof(_buffers[0]), "[rel %s]", vi.name->body);
                    return buffer;
                }

                if (vi.vmi.allocated) {
                    if (vi.vmi.offset >= 0) {
                        v->t = LIR_MEMORY;
                        v->storage.var.offset = vi.vmi.offset;
                        goto _shifted_to_memory;
                    }
                    else if (vi.vmi.reg >= 0) {
                        v->t = LIR_REGISTER;
                        v->storage.reg.reg = vi.vmi.reg;
                        goto _shifted_to_registers;
                    }
                }
            }

            return "<unknown>";
        }
        case LIR_MEMORY: {
_shifted_to_memory: {}
            const char* modifier = "qword";
            switch (v->size) {
                case 4: modifier = "dword"; break;
                case 2: modifier = "word";  break;
                case 1: modifier = "byte";  break;
                default: break;
            }

            if (v->storage.var.offset > 0) snprintf(buffer, sizeof(_buffers[0]), "%s [rbp - %d]", modifier, v->storage.var.offset);
            else snprintf(buffer, sizeof(_buffers[0]), "%s [rbp + %d]", modifier, ABS(v->storage.var.offset));
            return buffer;
        }
        case LIR_REGISTER: {
_shifted_to_registers: {}
            switch (LIR_format_register(v->storage.reg.reg, v->size)) {
                /* AVX registers */
                case XMM0: return "xmm0"; case XMM1: return "xmm1"; case XMM2: return "xmm2"; case XMM3: return "xmm3"; 
                case XMM4: return "xmm4";
                /* 64-bit */
                case RAX:  return "rax";  case RBX:  return "rbx";  case RCX:  return "rcx";  case RDX:  return "rdx";  
                case RSI:  return "rsi"; case RDI:  return "rdi";   case RBP:  return "rbp";  case RSP:  return "rsp";
                case R8:   return "r8";   case R9:   return "r9";   case R10:  return "r10";  case R11:  return "r11";  
                case R12:  return "r12";  case R13:  return "r13";  case R14:  return "r14"; case R15:  return "r15";
                /* 32-bit */
                case EAX:  return "eax";  case EBX:  return "ebx";  case ECX:  return "ecx";  case EDX:  return "edx";  
                case ESI:  return "esi"; case EDI:  return "edi";   case EBP:  return "ebp";  case ESP:  return "esp";
                case R8D:  return "r8d";  case R9D:  return "r9d";  case R10D: return "r10d"; case R11D: return "r11d"; 
                case R12D: return "r12d"; case R13D: return "r13d"; case R14D: return "r14d"; case R15D: return "r15d";
                /* 16-bit */
                case AX:   return "ax";   case BX:   return "bx";   case CX:   return "cx";   case DX:   return "dx";   
                case SI:   return "si";  case DI:   return "di";    case BP:   return "bp";   case SP:   return "sp";
                case R8W:  return "r8w";  case R9W:  return "r9w";  case R10W: return "r10w"; case R11W: return "r11w"; 
                case R12W: return "r12w"; case R13W: return "r13w"; case R14W: return "r14w"; case R15W: return "r15w"; 
                /* 8-bit */
                case AL:   return "al";   case BL:   return "bl";   case CL:   return "cl";   case DL:   return "dl";   
                case SIL:  return "sil"; case DIL:  return "dil";   case BPL:  return "bpl";  case SPL:  return "spl";  
                case AH:   return "ah";   case BH:   return "bh";   case CH:   return "ch";  case DH:   return "dh";
                case R8B:  return "r8b";  case R9B:  return "r9b";  case R10B: return "r10b"; case R11B: return "r11b"; 
                case R12B: return "r12b"; case R13B: return "r13b"; case R14B: return "r14b"; case R15B: return "r15b";
                default:   return "<unknown>";
            }
        }

        default: return "<unknown>";
    }

    return buffer;
}

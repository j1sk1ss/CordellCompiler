#include <asm/i386_gnu_nasm_asmgen.h>

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

static const char* _format_lir_register(lir_registers_t reg) {
    switch (reg) {
        /* AVX registers */
        case XMM0: return "xmm0"; case XMM1: return "xmm1"; case XMM2: return "xmm2"; case XMM3: return "xmm3"; 
        case XMM4: return "xmm4"; case XMM5: return "xmm5"; case XMM6: return "xmm6"; case XMM7: return "xmm7";
        /* 32-bit */
        case EAX:  return "eax";  case EBX:  return "ebx";  case ECX:  return "ecx";  case EDX:  return "edx";  
        case ESI:  return "esi";  case EDI:  return "edi";  case EBP:  return "ebp";  case ESP:  return "esp";
        /* 16-bit */
        case AX:   return "ax";   case BX:   return "bx";   case CX:   return "cx";   case DX:   return "dx";   
        case SI:   return "si";   case DI:   return "di";   case BP:   return "bp";   case SP:   return "sp";
        /* 8-bit */
        case AL:   return "al";   case BL:   return "bl";   case CL:   return "cl";   case DL:   return "dl";
        case AH:   return "ah";   case BH:   return "bh";   case CH:   return "ch";   case DH:   return "dh";
        default:   return "<unknown>";
    }
}

static const char* _get_mem_modifier(int size) {
    const char* modifier = "dword ";
    switch (size) {
        case 4: modifier = "dword "; break;
        case 2: modifier = "word ";  break;
        case 1: modifier = "byte ";  break;
        default: break;
    }
    
    return modifier;
}

const char* i386_gnu_nasm_format_lir_subject(lir_subject_t* v, sym_table_t* smt, int flag) {
    char* buffer = _get_buffer();
    if (!v) return "";
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
                    local  = "[rel _cpl_%s]";
                    global = "[rel %s]";
                }

                if (
                    fi.flags.entry
                ) snprintf(buffer, sizeof(_buffers[0]), global, fi.virt->body);
                else if (
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
                    snprintf(buffer, sizeof(_buffers[0]), "%s[rel %s]", _get_mem_modifier(v->size), vi.name->body);
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
            const char* modifier = _get_mem_modifier(v->size);
            if (flag == LEA_FLAG) modifier = "";
            const char* offset_base = _format_lir_register(v->storage.var.base);
            if (v->storage.var.offset > 0) snprintf(buffer, sizeof(_buffers[0]), "%s[%s - %d]", modifier, offset_base, v->storage.var.offset);
            else snprintf(buffer, sizeof(_buffers[0]), "%s[%s + %d]", modifier, offset_base, ABS(v->storage.var.offset));
            return buffer;
        }
        case LIR_REGISTER: {
_shifted_to_registers: {}
            if (flag == LDREF_FLAG) {
                snprintf(
                    buffer, sizeof(_buffers[0]), "%s[%s]", 
                    _get_mem_modifier(v->dsize), 
                    _format_lir_register(LIR_format_register(v->storage.reg.reg, 4))
                );
            }
            else {
                snprintf(
                    buffer, sizeof(_buffers[0]), "%s", 
                    _format_lir_register(LIR_format_register(v->storage.reg.reg, v->size))
                );
            }

            return buffer;
        }

        default: return "<unknown>";
    }

    return buffer;
}

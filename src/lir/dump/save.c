#include <lir/dump.h>

static const char* _registers_to_string(lir_registers_t reg) {
    switch(reg) {
        case XMM0: return "xmm0";
        case XMM1: return "xmm1";
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
        case EAX: return "eax";
        case EBX: return "ebx";
        case ECX: return "ecx";
        case EDX: return "edx";
        case ESI: return "esi";
        case EDI: return "edi";
        case EBP: return "ebp";
        case ESP: return "esp";
        case AX: return "ax";
        case BX: return "bx";
        case CX: return "cx";
        case DX: return "dx";
        case SI: return "si";
        case DI: return "di";
        case BP: return "bp";
        case SP: return "sp";
        case AL:  return "al";
        case BL:  return "bl";
        case CL:  return "cl";
        case DL:  return "dl";
        case SIL: return "sil";
        case DIL: return "dil";
        case BPL: return "bpl";
        case SPL: return "spl";
        case AH: return "ah";
        case BH: return "bh";
        case CH: return "ch";
        case DH: return "dh";
        default: return "??";
    }
}

static const char* _op_to_fmtstring(lir_operation_t op) {
    switch(op) {
        case LIR_BB:         return "\nBB%s: ";
        case LIR_ECLL: 
        case LIR_FCLL:       return "call %s;\n";
        case LIR_SYSC:       return "syscall;\n";
        case LIR_FRET:       return "return %s;\n";
        case LIR_MKLB:       return "%s:\n";
        case LIR_FDCL:       return "fn %s\n";
        case LIR_FEND:       return "fend\n";
        case LIR_STRT:       return "start\n";
        case LIR_STEND:      return "send\n";
        case LIR_PUSH:       return "push(%s);\n";
        case LIR_POP:        return "%s = pop();\n";
        case LIR_FEXT:       return "(fun) extern %s;\n";
        case LIR_OEXT:       return "(var) extern %s;\n";
        case LIR_TST:        return "test %s, %s;\n";
        case LIR_JNE:        return "jne %s;\n";
        case LIR_JE:         return "je %s;\n";
        case LIR_JMP:        return "jmp %s;\n";
        case LIR_CVTTSS2SI:  return "%s cvttss2si %s;\n";
        case LIR_CVTTSD2SI:  return "%s cvttsd2si %s;\n";
        case LIR_CVTSI2SS:   return "%s cvtsi2ss %s;\n";
        case LIR_CVTSI2SD:   return "%s cvtsi2sd %s;\n";
        case LIR_CVTSS2SD:   return "%s cvtss2sd %s;\n";
        case LIR_CVTSD2SS:   return "%s cvtsd2ss %s;\n";
        case LIR_MOVSX:      return "%s movsx %s;\n";
        case LIR_MOVZX:      return "%s movzx %s;\n";
        case LIR_MOVSXD:     return "%s movsxd %s;\n";
        case LIR_aMOV:       return "%s <<= %s;\n";
        case LIR_phiMOV:     return "[SSA] %s = %s;\n";
        case LIR_iMOV:       return "%s = %s;\n";
        case LIR_STARGLD:    return "%s = strt_loadarg();\n";
        case LIR_STARGRF:    return "%s = strt_ref_loadarg();\n";
        case LIR_VRDEALL:    return "kill(%s);\n";
        case LIR_STRDECL:    return "%s = str_alloc(%s);\n";
        case LIR_ARRDECL:    return "%s = arr_alloc(%s);\n";
        case LIR_VLADECL:    return "%s = vla(%s)\n";
        case LIR_STSARG:
        case LIR_STFARG:     return "stparam(%s);\n";
        case LIR_LOADFARG:   return "%s = ldparam();\n";
        case LIR_LOADFRET:   return "%s = fret();\n";
        case LIR_TF64:       return "%s = %s as f64;\n";
        case LIR_TF32:       return "%s = %s as f32;\n";
        case LIR_TI64:       return "%s = %s as i64;\n";
        case LIR_TI32:       return "%s = %s as i32;\n";
        case LIR_TI16:       return "%s = %s as i16;\n";
        case LIR_TI8:        return "%s = %s as i8;\n";
        case LIR_TU64:       return "%s = %s as u64;\n";
        case LIR_TU32:       return "%s = %s as u32;\n";
        case LIR_TU16:       return "%s = %s as u16;\n";
        case LIR_TU8:        return "%s = %s as u8;\n";
        case LIR_NEG:        return "%s = ~%s;\n";
        case LIR_NOT:        return "%s = !%s;\n";
        case LIR_INC:        return "%s++;\n";
        case LIR_DEC:        return "%s--;\n";
        case LIR_CMP:        return "cmp %s, %s;\n";
        case LIR_SETE:       return "sete %s;\n";
        case LIR_STNE:       return "stne %s;\n";
        case LIR_SETL:       return "setl %s;\n";
        case LIR_STLE:       return "stle %s;\n";
        case LIR_SETG:       return "setg %s;\n";
        case LIR_STGE:       return "stge %s;\n";
        case LIR_SETB:       return "setb %s;\n";
        case LIR_STBE:       return "stbe %s;\n";
        case LIR_SETA:       return "seta %s;\n";
        case LIR_STAE:       return "stae %s;\n";
        case LIR_bSHL:
        case LIR_iBLFT:      return "%s = %s << %s;\n";
        case LIR_bSHR:
        case LIR_iBRHT:      return "%s = %s >> %s;\n";
        case LIR_iLWR:       return "%s = %s < %s;\n";
        case LIR_iLRE:       return "%s = %s <= %s;\n";
        case LIR_iLRG:       return "%s = %s > %s;\n";
        case LIR_iLGE:       return "%s = %s >= %s;\n";
        case LIR_iCMP:       return "%s = %s == %s;\n";
        case LIR_iNMP:       return "%s = %s != %s;\n";
        case LIR_iOR:        return "%s = %s || %s;\n";
        case LIR_iAND:       return "%s = %s && %s;\n";
        case LIR_bOR:        return "%s = %s | %s;\n";
        case LIR_bXOR:       return "%s = %s ^ %s;\n";
        case LIR_bAND:       return "%s = %s & %s;\n";
        case LIR_iMOD:       return "%s = %s %% %s;\n";
        case LIR_iSUB:       return "%s = %s - %s;\n";
        case LIR_iDIV:       return "%s = %s / %s;\n";
        case LIR_iMUL:       return "%s = %s * %s;\n";
        case LIR_fADD:       return "%s = %s f+ %s;\n";
        case LIR_iADD:       return "%s = %s + %s;\n";
        case LIR_REF:        return "%s = &(%s);\n";
        case LIR_REF_GDREF:  return "%s = &(*%s);\n";
        case LIR_GDREF:      return "%s = *(%s);\n";
        case LIR_LDREF:      return "*(%s) = %s;\n";
        case LIR_RAW:        return "[raw] (link: %s), arg[%s];\n";
        case LIR_BREAKPOINT: return "== == brk %s == ==\n";
        case LIR_VRUSE:      return "use %s;\n";
        case LIR_EXITOP:     return "exit %s;\n";
        case LIR_MKSCOPE:
        case LIR_ENDSCOPE:   return "";
        default:             return "unknwn;\n";
    }
}

static char* _fmt_lir_subject(char* dst, lir_subject_t* s, sym_table_t* smt, int style) {
    if (!s) return dst;
    switch (s->t) {
        case LIR_MEMORY: {
            long off = s->storage.var.offset;
            dst += sprintf(dst, "[%s %s %ld]", _registers_to_string(s->storage.var.base), off > 0 ? "-" : "+", ABS(off)); 
            break;
        }
        case LIR_REGISTER: dst += sprintf(dst, "%s", _registers_to_string(s->storage.reg.reg)); break;
        case LIR_GLVARIABLE: {
            variable_info_t vi;
            if (VRTB_get_info_id(s->storage.var.v_id, &vi, &smt->v)) {
                dst += sprintf(dst, "%%%s", vi.name->body);
            }

            break;
        }
        case LIR_VARIABLE: dst += sprintf(dst, "%%%li", s->storage.var.v_id);      break;
        case LIR_NUMBER:   dst += sprintf(dst, "$%s", s->storage.num.value->body); break;
        case LIR_CONSTVAL: dst += sprintf(dst, "%ld", s->storage.cnst.value);      break;
        case LIR_LABEL:    dst += sprintf(dst, "lb%ld", s->storage.lb.lb_id);      break;
        case LIR_RAWASM:
        case LIR_STRING: {
            str_info_t si;
            if (STTB_get_info_id(s->storage.str.sid, &si, &smt->s)) {
                dst += sprintf(dst, "str(%s)", si.value->body);
            }

            break;
        }
        case LIR_FNAME: {
            func_info_t fi;
            if (!FNTB_get_info_id(s->storage.str.sid, &fi, &smt->f)) break;
            if (style != 1) dst += sprintf(dst, "%s", fi.virt->body);
            else {
                dst += sprintf(dst, "%s(", fi.virt->body);
                if (fi.args) {
                    for (ast_node_t* t = fi.args->c; t && t->t->t_type != SCOPE_TOKEN; t = t->siblings.n) {
                        ast_node_t* type = t;
                        if (type->t->t_type == VAR_ARGUMENTS_TOKEN) dst += sprintf(dst, "...");
                        else {
                            ast_node_t* name = t->c;
                            dst += sprintf(dst, "%s %s", DUMP_format_token(type->t), name->t->body->body);
                        }

                        if (t->siblings.n && t->siblings.n->t->t_type != SCOPE_TOKEN) {
                            dst += sprintf(dst, ", ");
                        }
                    }
                }

                dst += sprintf(dst, ")");
                if (fi.rtype) {
                    dst += sprintf(dst, " -> %s", DUMP_format_token(fi.rtype->t));
                }
            }

            break;
        }
        default: dst += sprintf(dst, "unknw"); break;
    }

    return dst;
}

static void _print_lir_block(const lir_block_t* block, sym_table_t* smt, int pos, int unused, FILE* output) {
    if (!block || (block->op == LIR_SETPOS && !pos)) return;
    
    int style = (block->op == LIR_FCLL || block->op == LIR_ECLL) ? 2 : 1;

    char arg1[256] = { 0 };
    if (block->farg) _fmt_lir_subject(arg1, block->farg, smt, style);

    char arg2[256] = { 0 };
    if (block->sarg) _fmt_lir_subject(arg2, block->sarg, smt, style);

    char arg3[256] = { 0 };
    if (block->targ) _fmt_lir_subject(arg3, block->targ, smt, style);

    if (block->unused && unused) fprintf(output, "[unused] ");

    const char* fmt = _op_to_fmtstring(block->op);
    char line[512] = { 0 };
    sprintf(line, fmt, arg1, arg2, arg3);
    fprintf(output, "%s", line);
}

int DUMP_format_lirctx(lir_ctx_t* ctx, sym_table_t* smt, int pos, int unused, FILE* output) {
    lir_block_t* lh = ctx->h;
    while (lh) {
        _print_lir_block(lh, smt, pos, unused, output);
        lh = lh->next;
    }

    return 1;
}

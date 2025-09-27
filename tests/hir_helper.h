#ifndef HIR_HELPER_H_
#define HIR_HELPER_H_

static const char* hir_op_to_string(hir_operation_t op) {
    switch(op) {
        case HIR_STARGLD:    return "STARGLD";
        case HIR_FARGST:     return "FARGST";
        case HIR_FARGLD:     return "FARGLD";
        case HIR_FCLL:       return "FCLL";
        case HIR_STORE_FCLL: return "HIR_STORE_FCLL";
        case HIR_ECLL:       return "ECLL";
        case HIR_STORE_ECLL: return "HIR_STORE_ECLL";
        case HIR_STRT:       return "STRT";
        case HIR_SYSC:       return "SYSC";
        case HIR_STORE_SYSC: return "HIR_STORE_SYSC";
        case HIR_FRET:       return "FRET";
        case HIR_MKLB:       return "MKLB";
        case HIR_FDCL:       return "FDCL";
        case HIR_FEND:       return "FEND";
        case HIR_OEXT:       return "OEXT";
        case HIR_JMP:        return "JMP";
        case HIR_iADD:       return "iADD";
        case HIR_iSUB:       return "iSUB";
        case HIR_iMUL:       return "iMUL";
        case HIR_iDIV:       return "iDIV";
        case HIR_iMOD:       return "iMOD";
        case HIR_iLRG:       return "iLRG";
        case HIR_iLGE:       return "iLGE";
        case HIR_iLWR:       return "iLWR";
        case HIR_iLRE:       return "iLRE";
        case HIR_iCMP:       return "iCMP";
        case HIR_iNMP:       return "iNMP";
        case HIR_iAND:       return "iAND";
        case HIR_iOR:        return "iOR";
        case HIR_iBLFT:      return "iBLFT";
        case HIR_iBRHT:      return "iBRHT";
        case HIR_bAND:       return "bAND";
        case HIR_bOR:        return "bOR";
        case HIR_bXOR:       return "bXOR";
        case HIR_RAW:        return "RAW";
        case HIR_IFOP:       return "IFOP";
        case HIR_NOT:        return "NOT";
        case HIR_STORE:      return "STORE";
        case HIR_VARDECL:    return "VARDECL";
        case HIR_ARRDECL:    return "ARRDECL";
        case HIR_STRDECL:    return "STRDECL";
        case HIR_PRMST:      return "PRMST";
        case HIR_PRMLD:      return "PRMLD";
        case HIR_PRMPOP:     return "PRMPOP";
        case HIR_STASM:      return "STASM";
        case HIR_ENDASM:     return "ENDASM";
        case HIR_GINDEX:     return "GINDEX";
        case HIR_LINDEX:     return "LINDEX";
        case HIR_GDREF:      return "GDREF";
        case HIR_LDREF:      return "LDREF";
        case HIR_REF:        return "REF";
        case HIR_EXITOP:     return "EXITOP";
        case HIR_CLNVRS:     return "HIR_CLNVRS";
        case HIR_IFLWOP:     return "HIR_IFLWOP";
        case HIR_IFLGOP:     return "HIR_IFLGOP";
        case HIR_STEND:      return "HIR_STEND";
        case HIR_MKSCOPE:    return "MKSCOPE";
        case HIR_ENDSCOPE:   return "ENDSCOPE";
        default: return "unknwop";
    }
}

static void print_hir_subject(const hir_subject_t* s) {
    if (!s) return;
    switch (s->t) {
        case HIR_STKVARSTR:  printf("strs: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARARR:  printf("arrs: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARF64:  printf("f64s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARU64:  printf("u64s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARI64:  printf("i64s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARF32:  printf("f32s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARU32:  printf("u32s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARI32:  printf("i32s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARU16:  printf("u16s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARI16:  printf("i16s: [vid=%d]", s->storage.var.v_id);   break;
        case HIR_STKVARU8:   printf("u8s: [vid=%d]", s->storage.var.v_id);    break;
        case HIR_STKVARI8:   printf("i8s: [vid=%d]", s->storage.var.v_id);    break;
        case HIR_TMPVARSTR:  printf("strt: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARARR:  printf("arrt: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARF64:  printf("f64t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARU64:  printf("u64t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARI64:  printf("i64t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARF32:  printf("f32t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARU32:  printf("u32t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARI32:  printf("i32t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARU16:  printf("u16t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARI16:  printf("i16t: [tid=%d]", s->storage.var.v_id);   break;
        case HIR_TMPVARU8:   printf("u8t: [tid=%d]", s->storage.var.v_id);    break;
        case HIR_TMPVARI8:   printf("i8t: [tid=%d]", s->storage.var.v_id);    break;
        case HIR_GLBVARSTR:  printf("strg: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARARR:  printf("arrg: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARF64:  printf("f64g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARU64:  printf("u64g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARI64:  printf("i64g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARF32:  printf("f32g: [gid=%i]", s->storage.var.v_id);   break;  
        case HIR_GLBVARU32:  printf("u32g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARI32:  printf("i32g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARU16:  printf("u16g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARI16:  printf("i16g: [gid=%i]", s->storage.var.v_id);   break;
        case HIR_GLBVARU8:   printf("i8g: [git=%i]", s->storage.var.v_id);    break;
        case HIR_GLBVARI8:   printf("u8g: [git=%i]", s->storage.var.v_id);    break;
        case HIR_NUMBER:     printf("%s", s->storage.num.value);              break;
        case HIR_CONSTVAL:   printf("%ld", s->storage.cnst.value);            break;
        case HIR_LABEL:      printf("lb: [id=%d]", s->id);                    break;
        case HIR_RAWASM:     printf("asm: [std_id=%i]", s->storage.str.s_id); break;
        case HIR_STRING:     printf("str: [std_id=%i]", s->storage.str.s_id); break;
        case HIR_FNAME:      printf("func: [fid=%i]", s->storage.str.s_id);   break;
        default: printf("unknw"); break;
    }
}

#endif
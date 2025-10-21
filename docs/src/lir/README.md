# LIR (x86_64) part
In the same way as during `HIR` generation, we now produce an intermediate representation similar to `3AC` — but using only two addresses. This step is relatively straightforward, as it primarily involves adapting instructions to the target machine’s addressing model. Because the exact implementation depends heavily on the target architecture (register count, instruction set, addressing modes, etc.), we typically don’t spend much time optimizing or generalizing this layer. Its main goal is simply to bridge the high-level `HIR` representation and the target-specific assembly form, ensuring that each instruction can be directly translated to a valid machine instruction.
![lir_gen](../../media/lir_gen.png)

Implementation of LIR generator is simple. We should translate HIR commands into LIR with some additional instructions.
```c
case HIR_STORE: {
    x86_64_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, -1, smt);
    x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, -1, smt);
    break;
}

case HIR_STARGLD: {
    int vrsize = LIR_get_hirtype_size(h->farg->t);
    switch (h->sarg->storage.cnst.value) {
        case 0: LIR_BLOCK2(ctx, LIR_iMOV, LIR_SUBJ_REG(RAX, vrsize), LIR_SUBJ_OFF(-8, vrsize)); break;
        case 1: LIR_BLOCK2(ctx, LIR_REF, LIR_SUBJ_REG(RAX, vrsize), LIR_SUBJ_OFF(-16, vrsize)); break;
    }

    x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, -1, smt);
    break;
}

case HIR_PHI_PREAMBLE: {
    x86_64_store_var_reg(LIR_iMOV, ctx, h->sarg, RAX, -1, smt);
    x86_64_load_var_reg(LIR_iMOV, ctx, h->farg, RAX, -1, smt);
    break;
}

case HIR_STRT:    LIR_BLOCK0(ctx, LIR_STRT);                                             break;
case HIR_STEND:   LIR_BLOCK0(ctx, LIR_STEND);                                            break;
case HIR_OEXT:    LIR_BLOCK1(ctx, LIR_OEXT, LIR_SUBJ_STRING(h->farg->storage.str.s_id)); break;
case HIR_MKSCOPE: scope_push(&scopes, h->farg->storage.cnst.value, offset);              break;
case HIR_EXITOP: {
    x86_64_deallocate_heap(ctx, scope_id_top(&scopes), &heap);
    LIR_BLOCK1(ctx, LIR_EXITOP, x86_64_format_variable(ctx, h->farg, smt));   
    break;
}

case HIR_ENDSCOPE: {
    scope_elem_t se;
    scope_pop_top(&scopes, &se);
    x86_64_deallocate_heap(ctx, h->farg->storage.cnst.value, &heap);
    stack_map_free_range(se.offset, -1, &ctx->stk);
    offset = se.offset;
    break;
}

case HIR_FRET: x86_64_deallocate_heap(ctx, scope_id_top(&scopes), &heap);
case HIR_SYSC:
case HIR_STORE_SYSC:
case HIR_FCLL:
case HIR_STORE_FCLL:
case HIR_ECLL:
case HIR_STORE_ECLL:
case HIR_FDCL:
case HIR_FARGLD: x86_64_generate_func(ctx, h, smt); break;
case HIR_FEND:   LIR_BLOCK0(ctx, LIR_FEND); break;

case HIR_STASM:
case HIR_RAW:
case HIR_ENDASM: x86_64_generate_asmblock(ctx, h, smt, &params); break;

case HIR_VRDEALL:
case HIR_VARDECL:
case HIR_ARRDECL:
case HIR_STRDECL: x86_64_generate_declaration(ctx, h, smt, &params, &scopes, &heap, &offset); break;

case HIR_REF:
case HIR_GDREF:
case HIR_LDREF: x86_64_generate_ref(ctx, h, smt); break;

case HIR_TF64:
case HIR_TF32:
case HIR_TI64:
case HIR_TI32:
case HIR_TI16:
case HIR_TI8:
case HIR_TU64:
case HIR_TU32:
case HIR_TU16:
case HIR_TU8: x86_64_generate_conv(ctx, h, smt); break;

case HIR_PRMST:
case HIR_FARGST: stack_push(&params, h->farg); break;

case HIR_JMP:  LIR_BLOCK1(ctx, LIR_JMP, LIR_SUBJ_LABEL(h->farg->id));  break;
case HIR_MKLB: LIR_BLOCK1(ctx, LIR_MKLB, LIR_SUBJ_LABEL(h->farg->id)); break;

case HIR_NOT: x86_64_generate_unary_op(ctx, h, smt); break;
case HIR_iBLFT:
case HIR_iBRHT:
case HIR_iLWR:
case HIR_iLRE:
case HIR_iLRG:
case HIR_iLGE:
case HIR_iCMP:
case HIR_iNMP:
case HIR_iOR:
case HIR_iAND:
case HIR_bOR:
case HIR_bXOR:
case HIR_bAND:
case HIR_iMOD:
case HIR_iSUB:
case HIR_iDIV: 
case HIR_iMUL: 
case HIR_iADD: x86_64_generate_binary_op(ctx, h, smt); break;

case HIR_IFOP:
case HIR_IFCPOP:
case HIR_IFNCPOP:
case HIR_IFLWOP:
case HIR_IFLWEOP:
case HIR_IFLGOP:
case HIR_IFLGEOP: x86_64_generate_ifop(ctx, h, smt); break;
```

## LIR x86_64 example
```
FDCL func [id=0]
iMOV R13D, EDI
iMOV R12D, ESI
iMOV [RBP - 8], R12D
iMOV [RBP - 4], R13D
iMOV RAX, 0
iMUL RAX, 4
REF RBX, [RBP - 8]
iADD RAX, RBX
GDREF RAX, RAX
iMOV R11D, EAX
iMOV RAX, 1
iMUL RAX, 4
REF RBX, [RBP - 8]
iADD RAX, RBX
GDREF RAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV EBX, R11D
iADD RAX, RBX
iMOV R11D, EAX
FRET R11D
STRT
iMOV RAX, [RBP + 8]
iMOV R13, RAX
REF RAX, [RBP + 16]
iMOV R13, RAX
iMOV RAX, 10
iMOV EAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV [RBP - 8], EAX
iMOV RAX, 10
iMOV EAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV [RBP - 16], EAX
iMOV RAX, 10
iMOV EAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV [RBP - 24], EAX
iMOV RAX, 10
iMOV EAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV R15D, EAX
iMOV RAX, 10
iMOV EAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV R14D, EAX
iMOV RAX, 10
iMOV EAX, RAX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV [RBP - 32], EAX
PUSH R11
PUSH R12
PUSH R13
PUSH R14
PUSH R15
iMOV EDI, [RBP - 16]
iMOV ESI, [RBP - 8]
FCLL func [id=41]
iMOV R13D, EAX
POP R15
POP R14
POP R13
POP R12
POP R11
iMOV EAX, [RBP - 8]
iMOV EBX, [RBP - 16]
iMUL RAX, RBX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV EBX, [RBP - 24]
iADD RAX, RBX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV EBX, R15D
iADD RAX, RBX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV EBX, R14D
iADD RAX, RBX
iMOV R11D, EAX
iMOV EAX, R11D
iMOV EBX, [RBP - 32]
iADD RAX, RBX
iMOV R11D, EAX
iMOV EAX, R13D
iMOV EBX, R11D
iCMP RAX, RBX
iMVZX RAX, AL
iMOV R11D, EAX
iCMP R11D, 0
JNE lb: [vid=73]
EXITOP 1
MKLB lb: [vid=73]
REF EAX, [RBP - 32]
iMOV R12, RAX
iMOV RAX, R12
iMOV EAX, RAX
iMOV R12D, EAX
iMOV EAX, R12D
iMOV R11D, EAX
EXITOP R11D
```

# LIR x86_64 optimization
`TODO`

# LIR (x86_64) part
In the same way as during `HIR` generation, we now produce an intermediate representation similar to `3AC` — but using only two addresses. This step is relatively straightforward, as it primarily involves adapting instructions to the target machine’s addressing model. Because the exact implementation depends heavily on the target architecture (register count, instruction set, addressing modes, etc.), we typically don’t spend much time optimizing or generalizing this layer. Its main goal is simply to bridge the high-level `HIR` representation and the target-specific assembly form, ensuring that each instruction can be directly translated to a valid machine instruction.
![lir_gen](../../media/lir_gen.png)

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

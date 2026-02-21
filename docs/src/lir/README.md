# LIR (x86_64) part
In the same way as during `HIR` generation, we now produce an intermediate representation similar to `3AC` — but using only two addresses. This step is relatively straightforward, as it primarily involves adapting instructions to the target machine’s addressing model. Because the exact implementation depends heavily on the target architecture (register count, instruction set, addressing modes, etc.), we typically don’t spend much time optimizing or generalizing this layer. Its main goal is simply to bridge the high-level `HIR` representation and the target-specific assembly form, ensuring that each instruction can be directly translated to a valid machine instruction.
![lir_gen](../../media/lir_gen.png)

Implementation of the LIR generator is simple. We should translate HIR commands into LIR with some additional instructions.
```c
// ...
case HIR_TF64: LIR_BLOCK2(ctx, LIR_TF64, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
case HIR_TF32: LIR_BLOCK2(ctx, LIR_TF32, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
case HIR_TI64: LIR_BLOCK2(ctx, LIR_TI64, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
case HIR_TI32: LIR_BLOCK2(ctx, LIR_TI32, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
case HIR_TI16: LIR_BLOCK2(ctx, LIR_TI16, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
case HIR_TI8:  LIR_BLOCK2(ctx, LIR_TI8,  x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
case HIR_TU64: LIR_BLOCK2(ctx, LIR_TU64, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
case HIR_TU32: LIR_BLOCK2(ctx, LIR_TU32, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
case HIR_TU16: LIR_BLOCK2(ctx, LIR_TU16, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
case HIR_TU8:  LIR_BLOCK2(ctx, LIR_TU8,  x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;

case HIR_JMP:  LIR_BLOCK1(ctx, LIR_JMP, LIR_SUBJ_LABEL(h->farg->id));  break;
case HIR_MKLB: LIR_BLOCK1(ctx, LIR_MKLB, LIR_SUBJ_LABEL(h->farg->id)); break;

case HIR_NOT: LIR_BLOCK2(ctx, LIR_NOT, x86_64_format_variable(h->farg), x86_64_format_variable(h->sarg)); break;
case HIR_IFOP2: {
    LIR_BLOCK2(ctx, LIR_CMP, x86_64_format_variable(h->farg), LIR_SUBJ_CONST(0));
    LIR_BLOCK1(ctx, LIR_JE, LIR_SUBJ_LABEL(h->targ->id));
    LIR_BLOCK1(ctx, LIR_JNE, LIR_SUBJ_LABEL(h->sarg->id));
    break;
}
// ...
```

## LIR x86_64 example
From the HIR we can produce a high level of the LIR:
```
BB27: fn foo(i32 a)
%12 = ldparam();
%5 = $1 as i32;
%4 = %12 + %5;
%13 = %4;


BB28: fn bar(i16 a) -> i32
%14 = ldparam();
%7 = $1 as i16;
%6 = %14 * %7;
%15 = %6;
return $1;

BB29: 
BB30: fn baz(u64 a) -> i0
%16 = ldparam();
%9 = $1 as u64;
%8 = %16 / %9;
%17 = %8;


BB31: fn fang(i32 a)
%18 = ldparam();
%11 = $1 as i32;
%10 = %18 + %11;
%19 = %10;


BB32: start
use $1;
stparam($1);
call foo(i32 a);
use $1;
stparam($1);
call bar(i16 a) -> i32;
use $1;
stparam($1);
call baz(u64 a) -> i0;
use $1;
stparam($1);
call fang(i32 a);
```

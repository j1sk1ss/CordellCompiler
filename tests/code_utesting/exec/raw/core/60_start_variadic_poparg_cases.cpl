: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM[args="AB"|args="CD"] :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

start(i64 argc, ptr ptr i8 argv) {
    putc(argv[1][0]);
    putc(argv[1][1]);
    exit 0;
}

:/ OUTPUT
@case_index=0
AB
---
@case_index=1
CD
---
/:

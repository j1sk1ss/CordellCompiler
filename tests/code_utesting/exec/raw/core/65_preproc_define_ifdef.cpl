: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
#define FLAG 0
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

start() {
#ifdef FLAG
    putc('D');
#endif
#ifndef FLAG
    putc('X');
#endif
    exit 0;
}

:/ OUTPUT
D
/:

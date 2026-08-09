: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

start() {
    arr words[2, ptr i8] = { ref "AX", ref "BY" };
    putc(words[0][1]);
    putc(words[1][0]);
    exit 0;
}

:/ OUTPUT
XB
/:

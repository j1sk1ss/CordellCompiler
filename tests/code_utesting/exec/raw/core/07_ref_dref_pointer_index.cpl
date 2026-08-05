: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

start() {
    i8 ch = 'Q';
    ptr i8 p = ref ch;

    putc(dref p);
    putc(p[0]);
    exit 0;
}

:/ OUTPUT
QQ
/:

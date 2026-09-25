: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: RUN_ASM :

function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

start() {
    defer putc('1');
    defer putc('2');
    defer putc('3');
    putc('A');
    exit 0;
}

:/ OUTPUT
A123
/:

: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: RUN_ASM :

function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

start() {
    defer putc('O');
    {
        defer putc('I');
        putc('A');
    }

    putc('B');
    exit 0;
}

:/ OUTPUT
ABOI
/:

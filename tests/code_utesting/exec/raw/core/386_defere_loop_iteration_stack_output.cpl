: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: RUN_ASM :

function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

function put_digit(i32 n) -> i0 {
    putc((n + '0') as i8);
}

start() {
    i32 i = 0;
    while i < 3; {
        defer put_digit(i);
        i += 1;
    }

    putc('A');
    exit 0;
}

:/ OUTPUT
A3
/:

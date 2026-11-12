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
    i32 value = 1;
    defer put_digit(value);
    value = 2;
    putc('A');
    exit 0;
}

:/ OUTPUT
A2
/:

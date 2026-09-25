: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: RUN_ASM :

function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

function pick(i32 value) -> i32 {
    defer putc('D');
    if value > 0; {
        putc('A');
        return 7;
    }

    return 4;
}

start() {
    i32 result = pick(1 as i32);
    putc((result + '0') as i8);
    exit 0;
}

:/ OUTPUT
AD7
/:

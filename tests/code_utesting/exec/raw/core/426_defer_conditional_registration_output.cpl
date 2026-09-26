: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: RUN_ASM :

function putc426(i8 c) -> i0 { syscall(RAW_SYS_WRITE, 1, ref c, 1); }

function run426(i32 flag) -> i0 {
    if flag; {
        defer putc426('T');
        putc426('A');
    } else {
        defer putc426('F');
        putc426('B');
    }
}

start() {
    run426(1);
    run426(0);
    exit 0;
}

:/ OUTPUT
ATFBTF
/:

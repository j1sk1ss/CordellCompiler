: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

function mark(i64 x) -> i64 {
    putc('X');
    return x;
}

start() {
    if 0 && mark(1); putc('A');
    else putc('B');
    exit 0;
}

:/ OUTPUT
B
/:

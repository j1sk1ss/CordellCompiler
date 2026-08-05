: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

function first(ptr i8 msg) -> i8 {
    return msg[0];
}

start() {
    putc(first(ref "Hello"));
    exit 0;
}

:/ OUTPUT
H
/:

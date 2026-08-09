: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    i64 x = ((1 as i64 << 3 as i64) | 2 as i64) ^ 1 as i64;
    x = x >> 1 as i64;
    put_digit(x);
    exit 0;
}

:/ OUTPUT
5
/:

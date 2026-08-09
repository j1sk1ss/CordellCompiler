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

function make_inc() -> ptr i0 {
    function inc(i64 x) -> i64 {
        return x + 1;
    }
    return inc;
}

start() {
    ptr i0 f = make_inc();
    put_digit(f(8));
    exit 0;
}

:/ OUTPUT
9
/:

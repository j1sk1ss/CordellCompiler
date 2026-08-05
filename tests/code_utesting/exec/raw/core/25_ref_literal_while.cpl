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
    ptr i8 s = ref "XYZ";
    i64 n = 0;
    while dref s; {
        n += 1;
        s += 1;
    }
    put_digit(n);
    exit 0;
}

:/ OUTPUT
3
/:

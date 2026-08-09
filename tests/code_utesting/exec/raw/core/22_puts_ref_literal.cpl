: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function strlen(ptr i8 s) -> i64 {
    i64 l = 0;
    while dref s; {
        l += 1;
        s += 1;
    }
    return l;
}

function puts(ptr i8 s) -> i0 {
    syscall(RAW_SYS_WRITE, 1, s, strlen(s));
}

start() {
    puts(ref "Hi");
    exit 0;
}

:/ OUTPUT
Hi
/:

: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}


start() {
    i64 out = 0;
    asm(out) {
        "mov %0, 65"
    }
    putc(out as i8);
    exit 0;
}

:/ OUTPUT
A
/:

: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
start() {
    i8 c = 'S';
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
    exit 0;
}

:/ OUTPUT
S
/:

: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

glob arr data[3, i8] = { 'A', 'B', 'C' };

start() {
    data[1] = 'Z';
    putc(data[0]);
    putc(data[1]);
    putc(data[2]);
    exit 0;
}

:/ OUTPUT
AZC
/:

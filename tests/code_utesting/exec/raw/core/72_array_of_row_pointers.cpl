: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

start() {
    arr row0[2, i8] = { 'A', 'B' };
    arr row1[2, i8] = { 'C', 'D' };
    arr rows[2, ptr i8] = { ref row0, ref row1 };

    putc(rows[0][1]);
    putc(rows[1][0]);
    exit 0;
}

:/ OUTPUT
BC
/:

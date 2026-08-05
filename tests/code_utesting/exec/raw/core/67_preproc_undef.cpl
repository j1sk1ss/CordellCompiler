: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
#define TEMP_FLAG 0
#undef TEMP_FLAG
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

start() {
#ifndef TEMP_FLAG
    putc('U');
#endif
#ifdef TEMP_FLAG
    putc('X');
#endif
    exit 0;
}

:/ OUTPUT
U
/:

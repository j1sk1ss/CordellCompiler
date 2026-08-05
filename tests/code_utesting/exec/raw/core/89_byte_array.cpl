: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

glob arr tape[8, i8];

start(i32 argc, ptr ptr i8 argv) {
    tape[0] = 'H';
    tape[1] = 'i';
    putc(tape[0]);
    putc(tape[1]);
    exit 0;
}

:/ OUTPUT
Hi
/:
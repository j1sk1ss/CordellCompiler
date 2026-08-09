: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

function foo() -> i0 {
    putc('G');
}

function bar() -> i0 {
    function foo() -> i0 {
        putc('L');
    }
    foo();
}

start() {
    bar();
    foo();
    exit 0;
}

:/ OUTPUT
LG
/:

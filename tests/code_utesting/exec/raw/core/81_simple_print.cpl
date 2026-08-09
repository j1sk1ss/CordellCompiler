: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: RUN_ASM :
function strlen(ptr i8 s) -> i32 {
    i32 l = 0;
    while dref s; {
        l += 1;
        s += 1;
    }

    return l;
}

start(i32 argc, ptr ptr i8 argv) {
    arr msg[0, i8] = "Hello, World!\n";
    syscall(RAW_SYS_WRITE, 1, ref msg, strlen(ref msg));
}

:/ OUTPUT
Hello, World!
/:
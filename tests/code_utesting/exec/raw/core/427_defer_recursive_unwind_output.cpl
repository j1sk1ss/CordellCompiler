: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: RUN_ASM :

function put_digit427(i32 n) -> i0 {
    i8 c = (n + '0') as i8;
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

function descend427(i32 n) -> i0 {
    defer put_digit427(n);
    if n > 0; { 
        descend427(n - 1); 
    }
}

start() {
    descend427(3);
    exit 0;
}

:/ OUTPUT
0123
/:

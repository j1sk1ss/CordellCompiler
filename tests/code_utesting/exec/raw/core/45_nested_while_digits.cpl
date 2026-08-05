: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}
function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    i64 i = 1;
    while i <= 2; {
        i64 j = 1;
        while j <= 2; {
            put_digit(i);
            put_digit(j);
            j += 1;
        }
        i += 1;
    }
    exit 0;
}

:/ OUTPUT
11122122
/:

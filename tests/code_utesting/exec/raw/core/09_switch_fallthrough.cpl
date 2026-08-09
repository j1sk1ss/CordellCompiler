: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

start() {
    i64 x = 1;
    switch x; {
        case 1; { putc('A'); }
        case 2; {
            putc('B');
            break;
        }
        default {
            putc('C');
            break;
        }
    }

    exit 0;
}

:/ OUTPUT
AB
/:

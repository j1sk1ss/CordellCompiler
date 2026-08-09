: ASM_ARCH=x86_64_nasm_macho,x86_64_nasm_gnu,i386_nasm_gnu :
#include "raw_syscall.inc"
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(RAW_SYS_WRITE, 1, ref c, 1);
}

start() {
    i8 x = 'B';
    @[no_fall]
    switch x; {
        case 'A'; {
            putc('X');
        }
        case 'B'; {
            putc('Y');
        }
        default {
            putc('Z');
        }
    }
    exit 0;
}

:/ OUTPUT
Y
/:

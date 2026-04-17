: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
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

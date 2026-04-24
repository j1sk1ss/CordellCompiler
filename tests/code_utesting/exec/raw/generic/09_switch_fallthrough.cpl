: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
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

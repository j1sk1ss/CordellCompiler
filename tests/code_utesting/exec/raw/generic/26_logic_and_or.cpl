: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    i64 a = 1;
    i64 b = 0;

    if a && not b; {
        putc('A');
    }
    else {
        putc('B');
    }

    if b || a; {
        putc('C');
    }
    else {
        putc('D');
    }

    exit 0;
}

:/ OUTPUT
AC
/:

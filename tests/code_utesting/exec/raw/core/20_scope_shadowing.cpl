: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    i8 x = 'A';

    {
        i8 x = 'B';
        putc(x);
    }

    putc(x);
    exit 0;
}

:/ OUTPUT
BA
/:

: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    i8 a = 'M';
    i8 b = 'N';
    arr ps[2, ptr i8] = { ref a, ref b };
    putc(dref ps[0]);
    putc(dref ps[1]);
    exit 0;
}

:/ OUTPUT
MN
/:

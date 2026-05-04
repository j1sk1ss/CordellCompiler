: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    i8 ch = 'Q';
    ptr i8 p = ref ch;

    putc(dref p);
    putc(p[0]);
    exit 0;
}

:/ OUTPUT
QQ
/:

: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    i8 c = 'K';
    ptr i8 p = ref c;
    ptr ptr i8 pp = ref p;

    putc((dref pp)[0]);
    exit 0;
}

:/ OUTPUT
K
/:

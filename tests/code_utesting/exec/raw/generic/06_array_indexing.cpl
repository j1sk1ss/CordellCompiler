: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

start() {
    arr a[3, i8] = { 'A', 'B', 'C' };
    putc(a[1]);
    exit 0;
}

:/ OUTPUT
B
/:

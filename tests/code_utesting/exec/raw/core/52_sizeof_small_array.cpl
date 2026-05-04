: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}
function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    arr a[3, i8] = { 'A', 'B', 'C' };
    put_digit(sizeof(a));
    exit 0;
}

:/ OUTPUT
3
/:

: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}
function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    i64 a = 7 / 2;
    i64 b = 7 % 2;
    put_digit(a);
    put_digit(b);
    exit 0;
}

:/ OUTPUT
31
/:

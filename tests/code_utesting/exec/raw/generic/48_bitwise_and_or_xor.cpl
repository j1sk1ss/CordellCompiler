: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}
function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    i64 a = 6 & 3;
    i64 b = 4 | 1;
    i64 c = 7 ^ 2;
    put_digit(a);
    put_digit(b);
    put_digit(c);
    exit 0;
}

:/ OUTPUT
255
/:

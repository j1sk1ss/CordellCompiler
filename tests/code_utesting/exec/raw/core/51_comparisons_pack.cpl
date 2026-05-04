: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}
function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    put_digit(2 > 1);
    put_digit(2 >= 2);
    put_digit(1 < 2);
    put_digit(2 <= 1);
    put_digit(3 == 3);
    put_digit(3 != 3);
    exit 0;
}

:/ OUTPUT
111010
/:

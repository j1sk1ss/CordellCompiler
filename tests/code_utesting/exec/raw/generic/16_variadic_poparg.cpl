: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

function sum2(...) -> i64 {
    @[poparg] i64 a;
    @[poparg] i64 b;
    return a + b;
}

start() {
    put_digit(sum2(2, 3));
    exit 0;
}

:/ OUTPUT
5
/:

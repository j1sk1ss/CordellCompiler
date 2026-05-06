: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    ptr i0 add = (i64 a, i64 b) => a + b;
    put_digit(add(2, 3));
    exit 0;
}

:/ OUTPUT
5
/:

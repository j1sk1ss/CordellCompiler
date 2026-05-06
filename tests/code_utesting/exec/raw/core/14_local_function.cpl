: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    function plus_two(i64 a) -> i64 {
        return a + 2;
    }

    put_digit(plus_two(5));
    exit 0;
}

:/ OUTPUT
7
/:

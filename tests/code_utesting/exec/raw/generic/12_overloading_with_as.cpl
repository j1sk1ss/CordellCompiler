: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

function pick() -> i64 {
    return 1;
}

function pick(i8 a) -> i64 {
    return 2;
}

function pick(i32 a) -> i64 {
    return 3;
}

start() {
    put_digit(pick());
    put_digit(pick(7 as i8));
    put_digit(pick(7 as i32));
    exit 0;
}

:/ OUTPUT
123
/:

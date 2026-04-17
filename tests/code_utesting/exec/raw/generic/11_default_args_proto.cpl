: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

function add(i64 a, i64 b = 2) -> i64;

function add(i64 a, i64 b = 2) -> i64 {
    return a + b;
}

start() {
    put_digit(add(3));
    exit 0;
}

:/ OUTPUT
5
/:

: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

function mul2(i64 x) -> i64;

start() {
    put_digit(mul2(3));
    exit 0;
}

function mul2(i64 x) -> i64 {
    return x * 2;
}

:/ OUTPUT
6
/:

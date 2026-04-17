: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

function base() -> i64 {
    return 4;
}

function add1(i64 x = base()) -> i64 {
    return x + 1;
}

start() {
    put_digit(add1());
    exit 0;
}

:/ OUTPUT
5
/:

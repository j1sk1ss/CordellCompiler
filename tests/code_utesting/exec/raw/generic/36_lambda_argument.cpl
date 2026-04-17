: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

function apply(ptr i0 f, i64 x) -> i64 {
    return f(x);
}

start() {
    put_digit(apply((i64 a) => a + 3, 2));
    exit 0;
}

:/ OUTPUT
5
/:

: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

start() {
    i64 x = ((1 as i64 << 3 as i64) | 2 as i64) ^ 1 as i64;
    x = x >> 1 as i64;
    put_digit(x);
    exit 0;
}

:/ OUTPUT
5
/:

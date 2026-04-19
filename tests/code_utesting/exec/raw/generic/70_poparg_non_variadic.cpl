: BUG :
: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

function pack(i32 a, i32 b) -> i32 {
    @[poparg] i32 x;
    @[poparg] i32 y;
    return x * 10 + y;
}

start() {
    i32 v = pack(3 as i32, 4 as i32);
    put_digit((v / 10) as i64);
    put_digit((v % 10) as i64);
    exit 0;
}

:/ OUTPUT
34
/:

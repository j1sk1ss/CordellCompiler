: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

function chloe(i32 a = 1) -> i64;
function max(i64 a = chloe(2)) -> i64;

function chloe(i32 a = 1) -> i64 {
    return a + 3;
}

function max(i64 a = chloe(2)) -> i64 {
    return a + 1;
}

start() {
    put_digit(max());
    exit 0;
}

:/ OUTPUT
6
/:

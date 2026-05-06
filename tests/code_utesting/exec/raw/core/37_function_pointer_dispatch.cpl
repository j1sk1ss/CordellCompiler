: BLOCK_TEST :
: RUN_ASM :
function putc(i8 c) -> i0 {
    syscall(0x2000004, 1, ref c, 1);
}

function put_digit(i64 x) -> i0 {
    putc(('0' + x) as i8);
}

function min(i64 a, i64 b) -> i64 {
    if a < b; return a;
    else return b;
}

function max(i64 a, i64 b) -> i64 {
    if a > b; return a;
    else return b;
}

function run(ptr i0 f, i64 a, i64 b) -> i64 {
    return f(a, b);
}

start() {
    put_digit(run(min, 2 as i64, 7 as i64));
    put_digit(run(max, 2 as i64, 7 as i64));
    exit 0;
}

:/ OUTPUT
27
/:

: BLOCK_TEST :
: RUN_ASM :

function add2(i64 x) -> i64 {
    return x + 2;
}

function mul3(i64 x) -> i64 {
    return x * 3;
}

function apply_twice(ptr i0 f, i64 x) -> i64 {
    return f(f(x));
}

start() {
    ptr i0 f = add2;
    ptr i0 g = mul3;
    exit (apply_twice(f, 5) + g(4)) as u8;
}

:/ OUTPUT
@exit_code=21
/:

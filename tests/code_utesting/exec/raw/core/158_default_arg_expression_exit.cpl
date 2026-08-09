: BLOCK_TEST :
: RUN_ASM :

function base() -> i64 {
    return 4;
}

function calc(i64 a, i64 b = base() * 3) -> i64;

function calc(i64 a, i64 b = base() * 3) -> i64 {
    return a + b;
}

start() {
    exit calc(5) as u8;
}

:/ OUTPUT
@exit_code=17
/:

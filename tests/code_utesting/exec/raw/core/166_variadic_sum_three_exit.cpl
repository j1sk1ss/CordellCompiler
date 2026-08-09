: BLOCK_TEST :
: RUN_ASM :

function sum3(...) -> i32 {
    @[poparg] i32 a;
    @[poparg] i32 b;
    @[poparg] i32 c;

    return a + b + c;
}

start() {
    exit sum3(9 as i32, 10 as i32, 11 as i32) as u8;
}

:/ OUTPUT
@exit_code=30
/:

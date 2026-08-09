: BLOCK_TEST :
: RUN_ASM :

function mix(...) -> i32 {
    @[poparg] i32 a;
    @[poparg] i8 b;
    @[poparg] u8 c;

    return a + b as i32 + c as i32;
}

start() {
    exit mix(10 as i32, 11 as i8, 21 as u8) as u8;
}

:/ OUTPUT
@exit_code=42
/:

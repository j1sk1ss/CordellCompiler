: BLOCK_TEST :
: RUN_ASM :

function make406() -> ptr i0 {
    ptr fn(i32)i32 result = (i32 value) => value * 2;
    return result;
}

start() {
    ptr fn(i32)i32 f = make406();
    exit f(21) as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

function apply405(ptr fn(i32)i32 f, i32 value) -> i32 {
    return f(value);
}

start() {
    ptr fn(i32)i32 add = (i32 value) => value + 7;
    exit apply405(add, 35) as u8;
}

:/ OUTPUT
@exit_code=42
/:

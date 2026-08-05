: BLOCK_TEST :
: RUN_ASM :

function id(i32 value) -> i32 {
    return value;
}

function twice(i32 value) -> i32 {
    return value * 2;
}

start() {
    arr funcs[2, ptr i0] = { id, twice };

    exit funcs[1](21 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

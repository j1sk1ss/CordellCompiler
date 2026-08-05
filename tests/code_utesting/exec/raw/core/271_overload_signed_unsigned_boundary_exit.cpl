: BLOCK_TEST :
: RUN_ASM :

function pick(i8 value) -> i32 {
    return value as i32 + 41;
}

function pick(u8 value) -> i32 {
    return value as i32 - 253;
}

start() {
    exit (pick((-1) as i8) + pick(255 as u8)) as u8;
}

:/ OUTPUT
@exit_code=42
/:

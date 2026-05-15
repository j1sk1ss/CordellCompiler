: RUN_ASM :

function inc(i32 value) -> i32 {
    return value + 1;
}

start() {
    ptr i0 fn = inc;
    exit fn(41) as u8;
}

:/ OUTPUT
@exit_code=42
/:

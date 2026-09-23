: RUN_ASM :

function inc(i32 value) -> i32 {
    return value + 1;
}

start() {
    ptr i0 fnc = inc;
    exit fnc(41 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

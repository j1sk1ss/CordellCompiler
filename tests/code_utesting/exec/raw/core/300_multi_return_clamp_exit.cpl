: BLOCK_TEST :
: RUN_ASM :

function clamp(i32 value) -> i32 {
    if value < 0; {
        return 0;
    }

    if value > 42; {
        return 42;
    }

    return value;
}

start() {
    exit clamp(100 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

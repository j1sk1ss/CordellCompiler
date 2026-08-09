: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 divisor = 0;
    i32 value = 42;

    if divisor != 0 && (100 / divisor) > 1; {
        value = 0;
    }

    exit value as u8;
}

:/ OUTPUT
@exit_code=42
/:

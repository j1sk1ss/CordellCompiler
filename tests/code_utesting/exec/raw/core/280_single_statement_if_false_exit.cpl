: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 value = 42;

    if value < 0; value = 1;

    exit value as u8;
}

:/ OUTPUT
@exit_code=42
/:

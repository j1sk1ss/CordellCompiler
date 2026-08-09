: BLOCK_TEST :
: RUN_ASM :

start() {
    ptr i8 data = 0 as ptr i8;
    i32 value = 0;

    if 1 || data[0] == 0; {
        value = 42;
    }

    exit value as u8;
}

:/ OUTPUT
@exit_code=42
/:

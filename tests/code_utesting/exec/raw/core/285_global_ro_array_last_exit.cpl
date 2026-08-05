: BLOCK_TEST :
: RUN_ASM :

glob arr values[3, i8] = { 1, 2, 42 };

start() {
    exit values[2] as u8;
}

:/ OUTPUT
@exit_code=42
/:

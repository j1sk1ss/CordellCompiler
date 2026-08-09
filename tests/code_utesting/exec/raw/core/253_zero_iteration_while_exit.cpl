: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 i = 10;
    i32 acc = 42;

    while i < 10; {
        acc = 0;
    }

    exit acc as u8;
}

:/ OUTPUT
@exit_code=42
/:

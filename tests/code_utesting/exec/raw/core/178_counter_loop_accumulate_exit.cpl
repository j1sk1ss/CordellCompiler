: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 x = 0;

    @[counter(6)]
    loop {
        x += 7;
    }

    exit x as u8;
}

:/ OUTPUT
@exit_code=42
/:

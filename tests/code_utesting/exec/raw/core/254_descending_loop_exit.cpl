: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 i = 6;
    i32 acc = 0;

    while i > 0; {
        acc += i;
        i -= 1;
    }

    exit (acc + 21) as u8;
}

:/ OUTPUT
@exit_code=42
/:

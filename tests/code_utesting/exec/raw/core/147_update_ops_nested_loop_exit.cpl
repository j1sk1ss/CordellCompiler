: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 i = 0;
    i32 acc = 1;

    while i < 4; {
        acc *= 2;
        acc += i;
        i += 1;
    }

    exit acc as u8;
}

:/ OUTPUT
@exit_code=27
/:

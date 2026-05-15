: RUN_ASM :

start() {
    i32 i = 0;
    i32 acc = 0;

    while i < 3; {
        i32 j = 0;

        while j < 4; {
            acc += i * j + 1;
            j += 1;
        }

        i += 1;
    }

    exit acc as u8;
}

:/ OUTPUT
@exit_code=30
/:

: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 i = 0;
    i32 acc = 0;

    while i < 4; {
        i32 j = 0;
        while j < 4; {
            if j == 2; {
                break;
            }

            acc += i + j;
            j += 1;
        }

        i += 1;
    }

    exit (acc + 26) as u8;
}

:/ OUTPUT
@exit_code=42
/:

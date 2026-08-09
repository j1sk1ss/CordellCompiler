: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 x = 9;
    i32 value = 42;

    switch x; {
        case 1; {
            value = 0;
        }
        case 2; {
            value = 1;
        }
    }

    exit value as u8;
}

:/ OUTPUT
@exit_code=42
/:

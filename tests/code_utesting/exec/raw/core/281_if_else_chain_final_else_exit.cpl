: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 value = 0;

    if value == 1; {
        value = 1;
    }
    else if value == 2; {
        value = 2;
    }
    else {
        value = 42;
    }

    exit value as u8;
}

:/ OUTPUT
@exit_code=42
/:

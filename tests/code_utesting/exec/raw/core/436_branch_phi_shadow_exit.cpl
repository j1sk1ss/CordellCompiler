: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 value = 0;
    i32 selector = 2;
    if selector == 1; {
        value = 10;
    } else if selector == 2; {
        i32 value = 21;
        value *= 2;
        selector = value;
    } else {
        value = 99;
    }
    exit (value + selector) as u8;
}

:/ OUTPUT
@exit_code=42
/:

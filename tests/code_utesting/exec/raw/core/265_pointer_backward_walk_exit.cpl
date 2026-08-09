: BLOCK_TEST :
: RUN_ASM :

start() {
    arr values[3, i8] = { 2, 40, 9 };
    ptr i8 p = ref values[2];

    p = p - 1;

    exit (dref p + 2) as u8;
}

:/ OUTPUT
@exit_code=42
/:

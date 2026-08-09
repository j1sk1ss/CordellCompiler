: BLOCK_TEST :
: RUN_ASM :

start() {
    arr values[4, i8] = { 1, 2, 3, 4 };
    ptr i8 p = ref values[0];

    p[3] = 42;

    exit values[3] as u8;
}

:/ OUTPUT
@exit_code=42
/:

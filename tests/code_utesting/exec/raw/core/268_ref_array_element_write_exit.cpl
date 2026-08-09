: BLOCK_TEST :
: RUN_ASM :

start() {
    arr values[3, i32] = { 1, 2, 3 };
    ptr i32 middle = ref values[1];

    dref middle = 42;

    exit values[1] as u8;
}

:/ OUTPUT
@exit_code=42
/:

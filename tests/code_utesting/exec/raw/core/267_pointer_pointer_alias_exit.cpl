: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 value = 10;
    ptr i32 p = ref value;
    ptr ptr i32 pp = ref p;
    ptr i32 alias = dref pp;

    dref alias += 32;

    exit value as u8;
}

:/ OUTPUT
@exit_code=42
/:

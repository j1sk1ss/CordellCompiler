: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 value = 11;
    ptr i32 p = ref value;
    ptr ptr i32 pp = ref p;

    ptr i32 q = dref pp;
    dref q = 44;

    exit value as u8;
}

:/ OUTPUT
@exit_code=44
/:

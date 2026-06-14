: BLOCK_TEST :
: RUN_ASM :

start() {
    u32 x = 1;
    x = x << 5;
    x |= 3;
    x = x >> 1;
    exit x as u8;
}

:/ OUTPUT
@exit_code=17
/:

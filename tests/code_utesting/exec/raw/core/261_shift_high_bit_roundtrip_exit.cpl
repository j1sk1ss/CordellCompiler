: BLOCK_TEST :
: RUN_ASM :

start() {
    u32 x = 1;
    u32 y = (x << 31) >> 31;

    exit (y + 41) as u8;
}

:/ OUTPUT
@exit_code=42
/:

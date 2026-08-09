: BLOCK_TEST :
: RUN_ASM :

start() {
    u64 x = 21;
    u64 y = (x << 0) + (x >> 0);

    exit y as u8;
}

:/ OUTPUT
@exit_code=42
/:

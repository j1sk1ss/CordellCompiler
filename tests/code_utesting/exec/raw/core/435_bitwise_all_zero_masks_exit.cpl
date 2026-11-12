: BLOCK_TEST :
: RUN_ASM :

start() {
    u32 all = 0xFFFFFFFF;
    u32 value = ((all & 0x2A) | (0xFF ^ 0xFF));
    exit value as u8;
}

:/ OUTPUT
@exit_code=42
/:

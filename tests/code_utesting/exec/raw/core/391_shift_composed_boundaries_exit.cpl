: BLOCK_TEST :
: RUN_ASM :

start() {
    u64 high = (1 as u64) << 63;
    u64 low = high >> 63;
    u64 mixed = ((0x55 as u64) << 8) | (0x2A as u64);
    exit (low + ((mixed >> 8) & 0xFF) - 44) as u8;
}

:/ OUTPUT
@exit_code=42
/:

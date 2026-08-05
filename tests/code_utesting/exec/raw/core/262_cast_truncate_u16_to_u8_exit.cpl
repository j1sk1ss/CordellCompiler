: BLOCK_TEST :
: RUN_ASM :

start() {
    u16 wide = 0x12A;
    u8 narrow = wide as u8;

    exit narrow;
}

:/ OUTPUT
@exit_code=42
/:

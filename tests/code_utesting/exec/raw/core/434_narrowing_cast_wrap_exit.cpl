: BLOCK_TEST :
: RUN_ASM :

start() {
    u16 wide = 300;
    u8 narrowed = wide as u8;
    i8 negative = -2;
    u8 wrapped = negative as u8;
    exit (narrowed + wrapped) as u8;
}

:/ OUTPUT
@exit_code=42
/:

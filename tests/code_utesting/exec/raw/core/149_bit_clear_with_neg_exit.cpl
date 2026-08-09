: BLOCK_TEST :
: RUN_ASM :

start() {
    i8 flags = 0x1F;
    flags &= neg 0x04;
    flags ^= 0x02;
    exit flags as u8;
}

:/ OUTPUT
@exit_code=25
/:

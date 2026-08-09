: BLOCK_TEST :
: RUN_ASM :

@[union]
container byte_pair {
    u16 word;
    arr bytes[2, u8];
}

start() {
    byte_pair v;
    v.word = 0;
    v.bytes[0] = 40;
    v.bytes[1] = 2;

    exit ((v.word & 0xFF) + ((v.word >> 8) & 0xFF)) as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

@[union]
container word_bytes {
    u32 word;
    arr bytes[4, u8];
}

start() {
    word_bytes w;
    w.word = 0;
    w.bytes[0] = 10;
    w.bytes[1] = 32;

    exit ((w.word & 0xFF) + ((w.word >> 8) & 0xFF)) as u8;
}

:/ OUTPUT
@exit_code=42
/:

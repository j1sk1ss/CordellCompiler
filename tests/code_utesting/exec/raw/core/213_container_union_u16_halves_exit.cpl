: BLOCK_TEST :
: RUN_ASM :

@[union]
container word_halves {
    u32 word;
    arr halves[2, u16];
}

start() {
    word_halves w;
    w.word = 0;
    w.halves[0] = 40;
    w.halves[1] = 2;

    exit ((w.word & 0xFFFF) + ((w.word >> 16) & 0xFFFF)) as u8;
}

:/ OUTPUT
@exit_code=42
/:

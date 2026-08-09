: BLOCK_TEST :
: RUN_ASM :

@[union]
container word_view {
    u32 word;
    arr bytes[4, u8];
}

start() {
    word_view w;
    w.word = 0;
    w.bytes[0] = 42;

    exit (w.word & 0xFF) as u8;
}

:/ OUTPUT
@exit_code=42
/:

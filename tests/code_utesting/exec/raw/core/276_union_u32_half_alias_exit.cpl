: BLOCK_TEST :
: RUN_ASM :

@[union]
container half_pair {
    u32 word;
    arr halves[2, u16];
}

start() {
    half_pair v;
    v.word = 0;
    v.halves[0] = 32;
    v.halves[1] = 10;

    exit ((v.word & 0xFFFF) + ((v.word >> 16) & 0xFFFF)) as u8;
}

:/ OUTPUT
@exit_code=42
/:

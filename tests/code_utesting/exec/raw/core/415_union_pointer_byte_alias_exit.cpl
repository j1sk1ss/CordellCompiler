: BLOCK_TEST :
: RUN_ASM :

@[union]
container view415 {
    u32 word;
    arr bytes[4, u8];
}

start() {
    view415 value;
    value.word = 0;
    ptr u8 p = ref value.bytes;
    p[0] = 19;
    p[1] = 23;
    exit ((value.word & 0xFF) + ((value.word >> 8) & 0xFF)) as u8;
}

:/ OUTPUT
@exit_code=42
/:

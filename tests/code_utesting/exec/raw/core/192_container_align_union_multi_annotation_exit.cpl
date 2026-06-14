: BLOCK_TEST :
: RUN_ASM :

@[align(1)]
@[union]
container half_word {
    u16 half;
    arr bytes[2, u8];
}

start() {
    half_word h;
    h.half = 0;
    h.bytes[0] = 21;
    h.bytes[1] = 21;

    exit ((h.half & 0xFF) + ((h.half >> 8) & 0xFF)) as u8;
}

:/ OUTPUT
@exit_code=42
/:

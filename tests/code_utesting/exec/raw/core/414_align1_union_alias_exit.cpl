: BLOCK_TEST :
: RUN_ASM :

@[align(1)]
@[union]
container bytes414 {
    u32 word;
    arr bytes[4, u8];
}

start() {
    bytes414 value;
    value.word = 0;
    value.bytes[0] = 40;
    value.bytes[3] = 2;
    exit (value.bytes[0] + value.bytes[3]) as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

@[union]
container byte_view {
    u32 word;
    arr bytes[4, u8];
}

start() {
    byte_view v;
    v.word = 0x00002A00;

    exit v.bytes[1] as u8;
}

:/ OUTPUT
@exit_code=42
/:

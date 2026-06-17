: BLOCK_TEST :
: RUN_ASM :

@[union]
container byte_packet {
    u32 word;
    arr bytes[4, u8];

    @[self]
    function low_sum(ptr byte_packet self) -> i32 {
        return self.bytes[0] as i32 + self.bytes[1] as i32;
    }
}

start() {
    byte_packet p;
    p.word = 0;
    p.bytes[0] = 19;
    p.bytes[1] = 23;

    exit p.low_sum() as u8;
}

:/ OUTPUT
@exit_code=42
/:

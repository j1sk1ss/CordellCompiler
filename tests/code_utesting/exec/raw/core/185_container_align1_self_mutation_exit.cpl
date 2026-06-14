: BLOCK_TEST :
: RUN_ASM :

@[align(1)]
container tiny_packet {
    i8  tag;
    i16 code;
    i32 value;

    @[self]
    function add(ptr tiny_packet self, i32 delta) -> i0 {
        self.value += delta;
    }
}

start() {
    tiny_packet p;
    p.tag = 5;
    p.code = 7;
    p.value = 20;
    p.add(10);

    exit (p.tag + p.code + p.value) as u8;
}

:/ OUTPUT
@exit_code=42
/:

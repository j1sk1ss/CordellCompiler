: BLOCK_TEST :
: RUN_ASM :

interface iface372 {
    @[self] @[abstract]
    function value(ptr iface372 self) -> i32;
}

@[union]
container impl372 implements iface372 {
    u32 word;
    arr bytes[4, u8];

    @[override]
    function value(ptr impl372 self) -> i32;
}

function impl372::value(ptr impl372 self) -> i32 {
    return ((self.word & 0xFF) + ((self.word >> 8) & 0xFF)) as i32;
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl372 item = place(ref storage, impl372);
    item.word = 0;
    item.bytes[0] = 19;
    item.bytes[1] = 23;

    ptr iface372 base = item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

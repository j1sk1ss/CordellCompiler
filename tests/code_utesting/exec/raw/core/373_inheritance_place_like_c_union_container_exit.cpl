: BLOCK_TEST :
: RUN_ASM :

interface iface373 {
    @[self] @[abstract]
    function value(ptr iface373 self) -> i32;
}

@[like_c]
@[union]
container impl373 implements iface373 {
    u64 qword;
    arr bytes[8, u8];

    @[override]
    function value(ptr impl373 self) -> i32;
}

function impl373::value(ptr impl373 self) -> i32 {
    return ((self.qword & 0xFF) + ((self.qword >> 8) & 0xFF)) as i32;
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl373 item = place(ref storage, impl373);
    item.qword = 0;
    item.bytes[0] = 20;
    item.bytes[1] = 22;

    ptr iface373 base = item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

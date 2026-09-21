: BLOCK_TEST :
: RUN_ASM :

@[union]
container view377 {
    u32 word;
    arr bytes[4, u8];
}

interface iface377 {
    @[self] @[abstract]
    function value(ptr iface377 self) -> i32;
}

@[like_c]
container impl377 implements iface377 {
    view377 view;

    @[override]
    function value(ptr impl377 self) -> i32;
}

function impl377::value(ptr impl377 self) -> i32 {
    return ((self.view.word & 0xFF) + ((self.view.word >> 8) & 0xFF)) as i32;
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl377 item = place(ref storage, impl377);
    item.view.word = 0;
    item.view.bytes[0] = 21;
    item.view.bytes[1] = 21;

    ptr iface377 base = item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

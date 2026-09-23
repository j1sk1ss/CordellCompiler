: BLOCK_TEST :
: RUN_ASM :

interface iface374 {
    @[self] @[abstract]
    function value(ptr iface374 self) -> i32;
}

@[align(16)]
container impl374 implements iface374 {
    i64 wide;
    i32 small;

    @[override]
    function value(ptr impl374 self) -> i32;
}

function impl374::value(ptr impl374 self) -> i32 {
    return (self.wide as i32) + self.small;
}

start() {
    @[align(16)] arr storage[128, u8];
    ptr impl374 item = place(ref storage, impl374);
    item.wide = 19 as i64;
    item.small = 23;

    ptr iface374 base = item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

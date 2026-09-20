: BLOCK_TEST :
: RUN_ASM :

interface iface354 {
    @[self] @[abstract]
    function value(ptr iface354 self) -> i32;
}

container impl354 implements iface354 {
    i32 base;

    @[override]
    function value(ptr impl354 self) -> i32;
}

function impl354::value(ptr impl354 self) -> i32 {
    return self.base + 5;
}

function call354(ptr iface354 item) -> i32 {
    return item.value();
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl354 item = place(ref storage, impl354);

    item.base = 37;
    exit call354(item) as u8;
}

:/ OUTPUT
@exit_code=42
/:

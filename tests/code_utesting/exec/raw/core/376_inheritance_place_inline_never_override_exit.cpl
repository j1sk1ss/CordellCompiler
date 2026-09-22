: BLOCK_TEST :
: RUN_ASM :

interface iface376 {
    @[self] @[abstract]
    function value(ptr iface376 self) -> i32;
}

container impl376 implements iface376 {
    i32 value_field;

    @[inline(never)]
    @[override]
    function value(ptr impl376 self) -> i32;
}

function impl376::value(ptr impl376 self) -> i32 {
    return self.value_field;
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl376 item = place(ref storage, impl376);
    item.value_field = 42;

    ptr iface376 base = item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

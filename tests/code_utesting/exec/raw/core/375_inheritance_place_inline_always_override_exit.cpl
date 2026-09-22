: BLOCK_TEST :
: RUN_ASM :

interface iface375 {
    @[self] @[abstract]
    function value(ptr iface375 self) -> i32;
}

container impl375 implements iface375 {
    i32 value_field;

    @[inline(always)]
    @[override]
    function value(ptr impl375 self) -> i32;
}

function impl375::value(ptr impl375 self) -> i32 {
    return self.value_field;
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl375 item = place(ref storage, impl375);
    item.value_field = 42;

    ptr iface375 base = item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

interface iface370 {
    @[self] @[abstract]
    function value(ptr iface370 self) -> i32;
}

@[like_c]
container impl370 implements iface370 {
    i8 tag;
    i32 value_field;

    @[override]
    function value(ptr impl370 self) -> i32;
}

function impl370::value(ptr impl370 self) -> i32 {
    return self.tag as i32 + self.value_field;
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl370 item = place(ref storage, impl370);
    item.tag = 2;
    item.value_field = 40;

    ptr iface370 base = item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

interface iface371 {
    @[self] @[abstract]
    function value(ptr iface371 self) -> i32;
}

@[align(1)]
container impl371 implements iface371 {
    i8 left;
    i32 right;

    @[override]
    function value(ptr impl371 self) -> i32;
}

function impl371::value(ptr impl371 self) -> i32 {
    return self.left as i32 + self.right;
}

start() {
    arr storage[64, u8];
    ptr impl371 item = place(ref storage, impl371);
    item.left = 12;
    item.right = 30;

    ptr iface371 base = item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

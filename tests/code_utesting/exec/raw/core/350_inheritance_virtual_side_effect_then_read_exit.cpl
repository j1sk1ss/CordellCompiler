: BLOCK_TEST :
: RUN_ASM :

interface iface350 {
    @[self] @[abstract]
    function bump(ptr iface350 self, i32 delta) -> i0;

    @[self] @[abstract]
    function value(ptr iface350 self) -> i32;
}
container impl350::iface350 {
    i32 current;
    @[override] function bump(ptr impl350 self, i32 delta) -> i0;
    @[override] function value(ptr impl350 self) -> i32;
}
function impl350::bump(ptr impl350 self, i32 delta) -> i0 { self.current += delta; }
function impl350::value(ptr impl350 self) -> i32 { return self.current; }

start() {
    impl350 item;
    item.current = 10;
    ptr iface350 base = ref item;
    base.bump(32 as i32);
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

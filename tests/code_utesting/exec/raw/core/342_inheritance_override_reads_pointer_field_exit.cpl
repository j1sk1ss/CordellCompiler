: BLOCK_TEST :
: RUN_ASM :

interface iface342 {
    @[self] @[abstract]
    function value(ptr iface342 self) -> i32;
}
container impl342 implements iface342 {
    ptr i32 source;
    @[override] function value(ptr impl342 self) -> i32;
}
function impl342::value(ptr impl342 self) -> i32 { return dref self.source; }

start() {
    i32 source = 42;
    impl342 item;
    item.source = ref source;
    ptr iface342 base = ref item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

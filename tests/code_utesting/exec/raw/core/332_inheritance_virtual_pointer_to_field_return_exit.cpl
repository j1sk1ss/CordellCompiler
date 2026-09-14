: BLOCK_TEST :
: RUN_ASM :

interface iface332 {
    @[self] @[abstract]
    function field(ptr iface332 self) -> ptr i32;
}
container impl332::iface332 {
    i32 value;
    @[override]
    function field(ptr impl332 self) -> ptr i32;
}
function impl332::field(ptr impl332 self) -> ptr i32 { return ref self.value; }

start() {
    impl332 item;
    item.value = 42 as i32;
    ptr iface332 base = ref item;
    ptr i32 p = base.field();
    exit (dref p) as u8;
}

:/ OUTPUT
@exit_code=42
/:

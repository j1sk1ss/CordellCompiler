: BLOCK_TEST :
: RUN_ASM :

interface iface331 {
    @[self] @[abstract]
    function update(ptr iface331 self, ptr i32 value) -> i0;
}
container impl331::iface331 {
    @[override]
    function update(ptr impl331 self, ptr i32 value) -> i0;
}
function impl331::update(ptr impl331 self, ptr i32 value) -> i0 {
    dref value += 12;
}

start() {
    impl331 item;
    i32 value = 30;
    ptr iface331 base = ref item;
    base.update(ref value);
    exit value as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

interface iface333 {
    @[self] @[abstract]
    function text(ptr iface333 self) -> ptr i8;
}
container impl333 implements iface333 { @[override] function text(ptr impl333 self) -> ptr i8; }
function impl333::text(ptr impl333 self) -> ptr i8 { return ref "A*Z"; }

start() {
    impl333 item;
    ptr iface333 base = ref item;
    ptr i8 text = base.text();
    exit text[1] as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

interface iface335 {
    @[self] @[abstract]
    function value(ptr iface335 self) -> i8;
}
container impl335 implements iface335 { @[override] function value(ptr impl335 self) -> i8; }
function impl335::value(ptr impl335 self) -> i8 { return 42 as i8; }

start() {
    impl335 item;
    ptr iface335 base = ref item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

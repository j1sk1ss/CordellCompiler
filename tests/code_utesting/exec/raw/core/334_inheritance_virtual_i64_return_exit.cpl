: BLOCK_TEST :
: RUN_ASM :

interface iface334 {
    @[self] @[abstract]
    function value(ptr iface334 self) -> i64;
}
container impl334::iface334 { @[override] function value(ptr impl334 self) -> i64; }
function impl334::value(ptr impl334 self) -> i64 { return 42; }

start() {
    impl334 item;
    ptr iface334 base = ref item;
    i64 result = base.value();
    exit result as u8;
}

:/ OUTPUT
@exit_code=42
/:

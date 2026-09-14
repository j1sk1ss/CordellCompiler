: BLOCK_TEST :
: RUN_ASM :

interface iface327 {
    @[self] @[abstract]
    function value(ptr iface327 self) -> i32;
}
container impl327::iface327 { @[override] function value(ptr impl327 self) -> i32; }
function impl327::value(ptr impl327 self) -> i32 { return 42; }

start() {
    impl327 item;
    ptr iface327 first = ref item;
    ptr iface327 second = first;
    exit second.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

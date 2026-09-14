: BLOCK_TEST :
: RUN_ASM :

function twice346(i32 value) -> i32 { return value * 2; }

interface iface346 {
    @[self] @[abstract]
    function operation(ptr iface346 self) -> ptr i0;
}
container impl346::iface346 { @[override] function operation(ptr impl346 self) -> ptr i0; }
function impl346::operation(ptr impl346 self) -> ptr i0 { return twice346; }

start() {
    impl346 item;
    ptr iface346 base = ref item;
    ptr i0 op = base.operation();
    exit op(21 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

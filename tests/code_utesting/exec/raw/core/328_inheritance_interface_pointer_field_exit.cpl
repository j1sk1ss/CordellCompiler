: BLOCK_TEST :
: RUN_ASM :

interface iface328 {
    @[self] @[abstract]
    function value(ptr iface328 self) -> i32;
}
container impl328::iface328 { @[override] function value(ptr impl328 self) -> i32; }
function impl328::value(ptr impl328 self) -> i32 { return 42; }

container holder328 {
    ptr iface328 item;
}

start() {
    impl328 item;
    holder328 holder;
    holder.item = ref item;
    exit holder.item.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

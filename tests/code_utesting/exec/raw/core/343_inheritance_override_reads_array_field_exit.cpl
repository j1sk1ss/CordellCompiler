: BLOCK_TEST :
: RUN_ASM :

interface iface343 {
    @[self] @[abstract]
    function sum(ptr iface343 self) -> i32;
}
container impl343 implements iface343 {
    arr values[3, i32];
    @[override] function sum(ptr impl343 self) -> i32;
}
function impl343::sum(ptr impl343 self) -> i32 {
    return self.values[0] + self.values[1] + self.values[2];
}

start() {
    impl343 item;
    item.values[0] = 10;
    item.values[1] = 12;
    item.values[2] = 20;
    ptr iface343 base = ref item;
    exit base.sum() as u8;
}

:/ OUTPUT
@exit_code=42
/:

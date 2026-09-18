: BLOCK_TEST :
: RUN_ASM :

interface iface344 {
    @[self] @[abstract]
    function at(ptr iface344 self, i32 index) -> i32;
}
container impl344 implements iface344 {
    arr values[3, i32];
    @[override] function at(ptr impl344 self, i32 index) -> i32;
}
function impl344::at(ptr impl344 self, i32 index) -> i32 { return self.values[index]; }

start() {
    impl344 item;
    item.values[0] = 1;
    item.values[1] = 42;
    item.values[2] = 3;
    ptr iface344 base = ref item;
    exit base.at(1 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

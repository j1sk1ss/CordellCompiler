: BLOCK_TEST :
: RUN_ASM :

interface iface339 {
    @[self] @[abstract]
    function step(ptr iface339 self) -> i32;
}
container impl339 implements iface339 { @[override] function step(ptr impl339 self) -> i32; }
function impl339::step(ptr impl339 self) -> i32 { return 7; }

function sum339(ptr iface339 item, i32 count) -> i32 {
    if count == 0; { return 0; }
    return item.step() + sum339(item, count - 1);
}

start() {
    impl339 item;
    exit sum339(ref item, 6 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

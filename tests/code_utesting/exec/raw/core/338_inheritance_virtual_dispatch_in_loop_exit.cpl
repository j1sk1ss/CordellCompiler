: BLOCK_TEST :
: RUN_ASM :

interface iface338 {
    @[self] @[abstract]
    function step(ptr iface338 self) -> i32;
}
container impl338::iface338 { @[override] function step(ptr impl338 self) -> i32; }
function impl338::step(ptr impl338 self) -> i32 { return 7; }

function run338(ptr iface338 item) -> i32 {
    i32 total = 0;
    i32 i = 0;
    while i < 6; {
        total += item.step();
        i += 1;
    }
    return total;
}

start() {
    impl338 item;
    exit run338(ref item) as u8;
}

:/ OUTPUT
@exit_code=42
/:

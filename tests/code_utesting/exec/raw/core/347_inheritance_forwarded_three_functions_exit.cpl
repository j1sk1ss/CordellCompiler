: BLOCK_TEST :
: RUN_ASM :

interface iface347 {
    @[self] @[abstract]
    function value(ptr iface347 self) -> i32;
}
container impl347 implements iface347 { @[override] function value(ptr impl347 self) -> i32; }
function impl347::value(ptr impl347 self) -> i32 { return 42; }

function level3_347(ptr iface347 item) -> i32 { return item.value(); }
function level2_347(ptr iface347 item) -> i32 { return level3_347(item); }
function level1_347(ptr iface347 item) -> i32 { return level2_347(item); }

start() {
    impl347 item;
    exit level1_347(ref item) as u8;
}

:/ OUTPUT
@exit_code=42
/:

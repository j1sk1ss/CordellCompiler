: BLOCK_TEST :
: RUN_ASM :

interface iface349 {
    @[self] @[abstract]
    function value(ptr iface349 self) -> i32;
}
container impl349 implements iface349 { @[override] function value(ptr impl349 self) -> i32; }
function impl349::value(ptr impl349 self) -> i32 { return 42; }

function run349(ptr iface349 item, i32 selector) -> i32 {
    i32 result = 0;
    @[no_fall] switch selector; {
        case 1; { result = item.value(); }
        default { result = 1; }
    }
    return result;
}

start() {
    impl349 item;
    exit run349(ref item, 1 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

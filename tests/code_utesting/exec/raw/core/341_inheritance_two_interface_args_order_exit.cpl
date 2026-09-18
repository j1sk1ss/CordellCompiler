: BLOCK_TEST :
: RUN_ASM :

interface iface341 {
    @[self] @[abstract]
    function value(ptr iface341 self) -> i32;
}
container first341 implements iface341 { @[override] function value(ptr first341 self) -> i32; }
container second341 implements iface341 { @[override] function value(ptr second341 self) -> i32; }
function first341::value(ptr first341 self) -> i32 { return 50; }
function second341::value(ptr second341 self) -> i32 { return 8; }

function diff341(ptr iface341 left, ptr iface341 right) -> i32 {
    return left.value() - right.value();
}

start() {
    first341 a;
    second341 b;
    exit diff341(ref a, ref b) as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

interface iface348 {
    @[self] @[abstract]
    function value(ptr iface348 self) -> i32;
}
container first348 implements iface348 { @[override] function value(ptr first348 self) -> i32; }
container second348 implements iface348 { @[override] function value(ptr second348 self) -> i32; }
function first348::value(ptr first348 self) -> i32 { return 5; }
function second348::value(ptr second348 self) -> i32 { return 42; }

start() {
    first348 a;
    second348 b;
    ptr iface348 item = ref a;
    i32 ignored = item.value();
    item = ref b;
    exit item.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

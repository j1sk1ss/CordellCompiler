: BLOCK_TEST :
: RUN_ASM :

interface iface352 {
    @[self] @[abstract]
    function value(ptr iface352 self) -> i32;
}
container impl352 implements iface352 {
    i32 state;
    @[override] function value(ptr impl352 self) -> i32;
}
function impl352::value(ptr impl352 self) -> i32 { return self.state; }

function sum352(ptr iface352 a, ptr iface352 b) -> i32 {
    return a.value() + b.value();
}

start() {
    impl352 a;
    impl352 b;
    a.state = 17;
    b.state = 25;
    exit sum352(ref a, ref b) as u8;
}

:/ OUTPUT
@exit_code=42
/:

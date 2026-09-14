: BLOCK_TEST :
: RUN_ASM :

interface root353 {
    @[self] @[abstract]
    function value(ptr root353 self) -> i32;
}
interface child353::root353 {
    @[self] @[abstract]
    function add(ptr child353 self, i32 delta) -> i0;
}
container impl353::child353 {
    i32 state;
    @[override] function value(ptr impl353 self) -> i32;
    @[override] function add(ptr impl353 self, i32 delta) -> i0;
}
function impl353::value(ptr impl353 self) -> i32 { return self.state; }
function impl353::add(ptr impl353 self, i32 delta) -> i0 { self.state += delta; }

function run353(ptr child353 item) -> i32 {
    item.add(12 as i32);
    return item.value();
}

start() {
    impl353 item;
    item.state = 30;
    exit run353(ref item) as u8;
}

:/ OUTPUT
@exit_code=42
/:

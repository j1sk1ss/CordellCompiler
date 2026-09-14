: BLOCK_TEST :
: RUN_ASM :

interface root337 {
    @[self] @[abstract]
    function first(ptr root337 self) -> i32;
}
interface child337::root337 {
    @[self] @[abstract]
    function second(ptr child337 self) -> i32;
}
container impl337::child337 {
    @[override] function first(ptr impl337 self) -> i32;
    @[override] function second(ptr impl337 self) -> i32;
}
function impl337::first(ptr impl337 self) -> i32 { return 20; }
function impl337::second(ptr impl337 self) -> i32 { return 22; }

function call337(ptr child337 item) -> i32 { return item.first() + item.second(); }

start() {
    impl337 item;
    exit call337(ref item) as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

interface root336 {
    @[self] @[abstract]
    function value(ptr root336 self) -> i32;
}
interface middle336::root336 { }
interface leaf336::middle336 { }
container impl336::leaf336 { @[override] function value(ptr impl336 self) -> i32; }
function impl336::value(ptr impl336 self) -> i32 { return 42; }

function call336(ptr root336 item) -> i32 { return item.value(); }

start() {
    impl336 item;
    exit call336(ref item) as u8;
}

:/ OUTPUT
@exit_code=42
/:

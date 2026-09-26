: BLOCK_TEST :
: RUN_ASM :

interface root420 {
    @[self] @[abstract] function left(ptr root420 self) -> i32;
}

interface child420 implements root420 {
    @[self] @[abstract] function right(ptr child420 self) -> i32;
}

container impl420 implements child420 {
    i32 base;
    @[override] function left(ptr impl420 self) -> i32;
    @[override] function right(ptr impl420 self) -> i32;
}

function impl420::left(ptr impl420 self) -> i32 { return self.base; }
function impl420::right(ptr impl420 self) -> i32 { return self.base + 2; }

start() {
    impl420 item;
    item.base = 20;
    ptr child420 child = ref item;
    exit (child.left() + child.right()) as u8;
}

:/ OUTPUT
@exit_code=42
/:

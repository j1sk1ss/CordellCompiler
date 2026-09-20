: BLOCK_TEST :
: RUN_ASM :

interface root356 {
    @[self] @[abstract]
    function value(ptr root356 self) -> i32;
}

interface child356 implements root356 {
    @[self] @[abstract]
    function add(ptr child356 self, i32 delta) -> i0;
}

container impl356 implements child356 {
    i32 state;

    @[override] function value(ptr impl356 self) -> i32;
    @[override] function add(ptr impl356 self, i32 delta) -> i0;
}

function impl356::value(ptr impl356 self) -> i32 { return self.state; }
function impl356::add(ptr impl356 self, i32 delta) -> i0 { self.state += delta; }

function bump356(ptr child356 item) -> i0 {
    item.add(22 as i32);
}

function read356(ptr root356 item) -> i32 {
    return item.value();
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl356 item = place(ref storage, impl356);

    item.state = 20;
    bump356(item);
    exit read356(item) as u8;
}

:/ OUTPUT
@exit_code=42
/:

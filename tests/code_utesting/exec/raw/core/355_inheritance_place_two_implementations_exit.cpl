: BLOCK_TEST :
: RUN_ASM :

interface iface355 {
    @[self] @[abstract]
    function value(ptr iface355 self) -> i32;
}

container add355 implements iface355 {
    i32 seed;

    @[override]
    function value(ptr add355 self) -> i32;
}

container mul355 implements iface355 {
    i32 seed;

    @[override]
    function value(ptr mul355 self) -> i32;
}

function add355::value(ptr add355 self) -> i32 { return self.seed + 3; }
function mul355::value(ptr mul355 self) -> i32 { return self.seed * 2; }

start() {
    @[align(8)] arr left_storage[64, u8];
    @[align(8)] arr right_storage[64, u8];

    ptr add355 left = place(ref left_storage, add355);
    ptr mul355 right = place(ref right_storage, mul355);

    left.seed = 15;
    right.seed = 12;

    arr items[2, ptr iface355];
    items[0] = left;
    items[1] = right;

    exit (items[0].value() + items[1].value()) as u8;
}

:/ OUTPUT
@exit_code=42
/:

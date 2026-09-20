: BLOCK_TEST :
: RUN_ASM :

interface value361 {
    @[self] @[abstract]
    function get(ptr value361 self) -> i32;
    @[self] @[abstract]
    function add_other(ptr value361 self, ptr value361 other) -> i32;
}

container impl361 implements value361 {
    i32 value;

    @[override] function get(ptr impl361 self) -> i32;
    @[override] function add_other(ptr impl361 self, ptr value361 other) -> i32;
}

function impl361::get(ptr impl361 self) -> i32 {
    return self.value;
}

function impl361::add_other(ptr impl361 self, ptr value361 other) -> i32 {
    return self.value + other.get();
}

start() {
    @[align(8)] arr left_storage[64, u8];
    @[align(8)] arr right_storage[64, u8];
    ptr impl361 left = place(ref left_storage, impl361);
    ptr impl361 right = place(ref right_storage, impl361);

    left.value = 17;
    right.value = 25;

    ptr value361 base = left;
    exit base.add_other(right) as u8;
}

:/ OUTPUT
@exit_code=42
/:

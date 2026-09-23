: BLOCK_TEST :
: RUN_ASM :

interface root363 {
    @[self] @[abstract]
    function root_value(ptr root363 self) -> i32;
}

interface middle363 implements root363 { }

interface leaf363 implements middle363 {
    @[self] @[abstract]
    function leaf_value(ptr leaf363 self) -> i32;
}

container impl363 implements leaf363 {
    i32 value;

    @[override] function root_value(ptr impl363 self) -> i32;
    @[override] function leaf_value(ptr impl363 self) -> i32;
}

function impl363::root_value(ptr impl363 self) -> i32 { return self.value; }
function impl363::leaf_value(ptr impl363 self) -> i32 { return self.value + 1; }

function read_root363(ptr root363 item) -> i32 {
    return item.root_value();
}

function read_leaf363(ptr leaf363 item) -> i32 {
    return item.leaf_value();
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl363 item = place(ref storage, impl363);

    item.value = 20;
    exit (read_root363(item) + read_leaf363(item)) as u8;
}

:/ OUTPUT
@exit_code=41
/:

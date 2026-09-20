: BLOCK_TEST :
: RUN_ASM :

interface counter357 {
    @[self] @[abstract]
    function add(ptr counter357 self, i32 value) -> i0;
    @[self] @[abstract]
    function value(ptr counter357 self) -> i32;
}

container impl357 implements counter357 {
    i32 total;

    @[override] function add(ptr impl357 self, i32 value) -> i0;
    @[override] function value(ptr impl357 self) -> i32;
}

function impl357::add(ptr impl357 self, i32 value) -> i0 {
    self.total += value;
}

function impl357::value(ptr impl357 self) -> i32 {
    return self.total;
}

function run357(ptr counter357 item) -> i32 {
    item.add(11 as i32);
    item.add(13 as i32);
    return item.value();
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl357 item = place(ref storage, impl357);

    item.total = 18;
    exit run357(item) as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

interface value359 {
    @[self] @[abstract]
    function get(ptr value359 self) -> i32;
}

container first359 implements value359 {
    i32 value;

    @[override] function get(ptr first359 self) -> i32;
}

container second359 implements value359 {
    i32 value;

    @[override] function get(ptr second359 self) -> i32;
}

function first359::get(ptr first359 self) -> i32 { return self.value + 10; }
function second359::get(ptr second359 self) -> i32 { return self.value + 20; }

start() {
    @[align(8)] arr first_storage[64, u8];
    @[align(8)] arr second_storage[64, u8];

    ptr first359 first = place(ref first_storage, first359);
    ptr second359 second = place(ref second_storage, second359);

    first.value = 5;
    second.value = 22;

    ptr value359 item = first;
    i32 total = item.get();
    item = second;
    total += item.get();

    exit (total - 15) as u8;
}

:/ OUTPUT
@exit_code=42
/:

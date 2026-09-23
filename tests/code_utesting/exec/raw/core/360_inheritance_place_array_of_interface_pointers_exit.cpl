: BLOCK_TEST :
: RUN_ASM :

interface value360 {
    @[self] @[abstract]
    function get(ptr value360 self) -> i32;
}

container impl360 implements value360 {
    i32 value;

    @[override]
    function get(ptr impl360 self) -> i32;
}

function impl360::get(ptr impl360 self) -> i32 {
    return self.value;
}

start() {
    @[align(8)] arr first_storage[64, u8];
    @[align(8)] arr second_storage[64, u8];
    ptr impl360 first = place(ref first_storage, impl360);
    ptr impl360 second = place(ref second_storage, impl360);

    first.value = 19;
    second.value = 23;

    arr items[2, ptr value360];
    items[0] = first;
    items[1] = second;

    exit (items[0].get() + items[1].get()) as u8;
}

:/ OUTPUT
@exit_code=42
/:

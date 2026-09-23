: ASM_ARCH=x86_64_nasm_gnu :
: RUN_ASM :

interface value23 {
    @[self] @[abstract]
    function get(ptr value23 self) -> i32;
}

container impl23 implements value23 {
    i32 value;

    @[override]
    function get(ptr impl23 self) -> i32;
}

function impl23::get(ptr impl23 self) -> i32 {
    return self.value;
}

start() {
    @[align(8)] arr first_storage[64, u8];
    @[align(8)] arr second_storage[64, u8];
    @[align(8)] arr third_storage[64, u8];

    ptr impl23 first = place(ref first_storage, impl23);
    ptr impl23 second = place(ref second_storage, impl23);
    ptr impl23 third = place(ref third_storage, impl23);

    first.value = 10;
    second.value = 14;
    third.value = 18;

    arr items[3, ptr value23];
    items[0] = first;
    items[1] = second;
    items[2] = third;

    i32 total = 0;
    i32 i = 0;
    while i < 3; {
        total += items[i].get();
        i += 1;
    }

    exit total as u8;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=42
/:

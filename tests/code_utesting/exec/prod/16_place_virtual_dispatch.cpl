: ASM_ARCH=x86_64_nasm_gnu :
: RUN_ASM :

interface value16 {
    @[self] @[abstract]
    function get(ptr value16 self) -> i32;
}

container impl16 implements value16 {
    i32 value;

    @[override]
    function get(ptr impl16 self) -> i32;
}

function impl16::get(ptr impl16 self) -> i32 {
    return self.value;
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl16 item = place(ref storage, impl16);
    item.value = 42;

    ptr value16 base = item;
    exit base.get() as u8;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=42
/:

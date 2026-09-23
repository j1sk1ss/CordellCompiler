: ASM_ARCH=x86_64_nasm_gnu :
: RUN_ASM :

interface value21 {
    @[self] @[abstract]
    function get(ptr value21 self) -> i32;
}

@[like_c]
container impl21 implements value21 {
    i8 tag;
    i32 value;

    @[override]
    function get(ptr impl21 self) -> i32;
}

function impl21::get(ptr impl21 self) -> i32 {
    return self.tag as i32 + self.value;
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl21 item = place(ref storage, impl21);
    item.tag = 7;
    item.value = 35;

    ptr value21 base = item;
    exit base.get() as u8;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=42
/:

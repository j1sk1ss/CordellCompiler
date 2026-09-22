: ASM_ARCH=x86_64_nasm_gnu :
: RUN_ASM :

interface value20 {
    @[self] @[abstract]
    function get(ptr value20 self) -> i32;
}

container first20 implements value20 {
    i32 value;

    @[override] function get(ptr first20 self) -> i32;
}

container second20 implements value20 {
    i32 value;

    @[override] function get(ptr second20 self) -> i32;
}

function first20::get(ptr first20 self) -> i32 { return self.value + 100; }
function second20::get(ptr second20 self) -> i32 { return self.value + 2; }

start() {
    @[align(8)] arr storage[64, u8];

    ptr first20 first = place(ref storage, first20);
    first.value = 1;

    ptr second20 second = place(ref storage, second20);
    second.value = 40;

    ptr value20 base = second;
    exit base.get() as u8;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=42
/:

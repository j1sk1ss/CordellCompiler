: ASM_ARCH=x86_64_nasm_gnu :
: RUN_ASM :

interface value24 {
    @[self] @[abstract]
    function get(ptr value24 self) -> i32;
    @[self] @[abstract]
    function add_other(ptr value24 self, ptr value24 other) -> i32;
}

container impl24 implements value24 {
    i32 value;

    @[override] function get(ptr impl24 self) -> i32;
    @[override] function add_other(ptr impl24 self, ptr value24 other) -> i32;
}

function impl24::get(ptr impl24 self) -> i32 {
    return self.value;
}

function impl24::add_other(ptr impl24 self, ptr value24 other) -> i32 {
    return self.value + other.get();
}

start() {
    @[align(8)] arr left_storage[64, u8];
    @[align(8)] arr right_storage[64, u8];

    ptr impl24 left = place(ref left_storage, impl24);
    ptr impl24 right = place(ref right_storage, impl24);
    left.value = 17;
    right.value = 25;

    ptr value24 base = left;
    exit base.add_other(right) as u8;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=42
/:

: ASM_ARCH=x86_64_nasm_gnu :
: RUN_ASM :

interface op17 {
    @[self] @[abstract]
    function apply(ptr op17 self, i32 value) -> i32;
}

container add17 implements op17 {
    i32 bias;

    @[override]
    function apply(ptr add17 self, i32 value) -> i32;
}

container mul17 implements op17 {
    i32 factor;

    @[override]
    function apply(ptr mul17 self, i32 value) -> i32;
}

function add17::apply(ptr add17 self, i32 value) -> i32 { return value + self.bias; }
function mul17::apply(ptr mul17 self, i32 value) -> i32 { return value * self.factor; }

start() {
    @[align(8)] arr add_storage[64, u8];
    @[align(8)] arr mul_storage[64, u8];

    ptr add17 add = place(ref add_storage, add17);
    ptr mul17 mul = place(ref mul_storage, mul17);
    add.bias = 5;
    mul.factor = 3;

    arr pipeline[2, ptr op17];
    pipeline[0] = add;
    pipeline[1] = mul;

    i32 value = 9;
    value = pipeline[0].apply(value);
    value = pipeline[1].apply(value);

    exit value as u8;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=42
/:

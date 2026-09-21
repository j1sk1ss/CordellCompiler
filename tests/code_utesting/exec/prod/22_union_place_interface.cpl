: ASM_ARCH=x86_64_nasm_gnu :
: RUN_ASM :

interface value22 {
    @[self] @[abstract]
    function get(ptr value22 self) -> i32;
}

@[union]
container impl22 implements value22 {
    u32 word;
    arr bytes[4, u8];

    @[override]
    function get(ptr impl22 self) -> i32;
}

function impl22::get(ptr impl22 self) -> i32 {
    return ((self.word & 0xFF) + ((self.word >> 8) & 0xFF)) as i32;
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl22 item = place(ref storage, impl22);
    item.word = 0 as u32;
    item.bytes[0] = 19;
    item.bytes[1] = 23;

    ptr value22 base = item;
    exit base.get() as u8;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=42
/:

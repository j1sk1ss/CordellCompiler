: ASM_ARCH=x86_64_nasm_gnu :
: RUN_ASM :

interface value19 {
    @[self] @[abstract]
    function get(ptr value19 self) -> i32;
}

container impl19 implements value19 {
    i32 value;

    @[override]
    function get(ptr impl19 self) -> i32;
}

function impl19::get(ptr impl19 self) -> i32 {
    return self.value;
}

function memcpy19(ptr i0 dst, ptr i0 src, i64 count) -> ptr i0 {
    ptr u8 d = dst as ptr u8;
    ptr u8 s = src as ptr u8;
    i64 i = 0;

    while i < count; {
        d[i] = s[i];
        i += 1;
    }

    return dst;
}

start() {
    @[align(8)] arr src_storage[64, u8];
    @[align(8)] arr dst_storage[64, u8];
    ptr impl19 src = place(ref src_storage, impl19);

    src.value = 42;
    memcpy19(ref dst_storage, ref src_storage, sizeof(impl19));

    ptr impl19 dst = (ref dst_storage) as ptr impl19;
    ptr value19 base = dst;
    exit base.get() as u8;
}

:/ OUTPUT
@measure_time
@measure_lines
@repeat=5
@exit_code=42
/:

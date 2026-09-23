: BLOCK_TEST :
: RUN_ASM :

interface iface378 {
    @[self] @[abstract]
    function value(ptr iface378 self) -> i32;
}

@[like_c]
container impl378 implements iface378 {
    i8 tag;
    i32 value_field;

    @[override]
    function value(ptr impl378 self) -> i32;
}

function impl378::value(ptr impl378 self) -> i32 {
    return self.tag as i32 + self.value_field;
}

function memcpy378(ptr i0 dst, ptr i0 src, i64 count) -> ptr i0 {
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

    ptr impl378 src = place(ref src_storage, impl378);
    src.tag = 7;
    src.value_field = 35;

    memcpy378(ref dst_storage, ref src_storage, sizeof(impl378));

    ptr impl378 dst = (ref dst_storage) as ptr impl378;
    ptr iface378 base = dst;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

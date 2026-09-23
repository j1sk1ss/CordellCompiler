: BLOCK_TEST :
: RUN_ASM :

interface iface364 {
    @[self] @[abstract]
    function value(ptr iface364 self) -> i32;
}

container impl364 implements iface364 {
    i32 value_field;

    @[override]
    function value(ptr impl364 self) -> i32;
}

function impl364::value(ptr impl364 self) -> i32 {
    return self.value_field;
}

function memcpy364(ptr i0 dst, ptr i0 src, i64 count) -> ptr i0 {
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

    ptr impl364 src = place(ref src_storage, impl364);
    src.value_field = 42;

    memcpy364(ref dst_storage, ref src_storage, sizeof(impl364));

    ptr impl364 dst = (ref dst_storage) as ptr impl364;
    ptr iface364 item = dst;
    exit item.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

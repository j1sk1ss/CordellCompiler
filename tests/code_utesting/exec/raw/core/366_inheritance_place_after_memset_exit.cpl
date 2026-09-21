: BLOCK_TEST :
: RUN_ASM :

interface iface366 {
    @[self] @[abstract]
    function value(ptr iface366 self) -> i32;
}

container impl366 implements iface366 {
    i32 value_field;

    @[override]
    function value(ptr impl366 self) -> i32;
}

function impl366::value(ptr impl366 self) -> i32 {
    return self.value_field;
}

function memset366(ptr i0 dst, u8 value, i64 count) -> ptr i0 {
    ptr u8 d = dst as ptr u8;
    i64 i = 0;

    while i < count; {
        d[i] = value;
        i += 1;
    }

    return dst;
}

start() {
    @[align(8)] arr storage[64, u8];
    memset366(ref storage, 0x7F as u8, 64);

    ptr impl366 item = place(ref storage, impl366);
    item.value_field = 42;

    ptr iface366 base = item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

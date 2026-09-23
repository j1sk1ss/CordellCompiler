: BLOCK_TEST :
: RUN_ASM :

interface iface369 {
    @[self] @[abstract]
    function value(ptr iface369 self) -> i32;
}

container impl369 implements iface369 {
    i32 value_field;

    @[override]
    function value(ptr impl369 self) -> i32;
}

function impl369::value(ptr impl369 self) -> i32 {
    return self.value_field;
}

function memmove369(ptr i0 dst, ptr i0 src, i64 count) -> ptr i0 {
    ptr u8 d = dst as ptr u8;
    ptr u8 s = src as ptr u8;

    if d < s; {
        i64 i = 0;
        while i < count; {
            d[i] = s[i];
            i += 1;
        }
    } else {
        i64 i = count;
        while i > 0; {
            i -= 1;
            d[i] = s[i];
        }
    }

    return dst;
}

start() {
    @[align(8)] arr arena[128, u8];
    ptr u8 src_addr = ref arena;
    ptr u8 dst_addr = ref arena;
    dst_addr = dst_addr + 32;

    ptr impl369 src = place(src_addr, impl369);
    src.value_field = 42;

    memmove369(dst_addr, src_addr, sizeof(impl369));

    ptr impl369 dst = dst_addr as ptr impl369;
    ptr iface369 item = dst;
    exit item.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

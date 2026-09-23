: BLOCK_TEST :
: RUN_ASM :

interface iface365 {
    @[self] @[abstract]
    function value(ptr iface365 self) -> i32;
}

container first365 implements iface365 {
    i32 value_field;

    @[override] function value(ptr first365 self) -> i32;
}

container second365 implements iface365 {
    i32 value_field;

    @[override] function value(ptr second365 self) -> i32;
}

function first365::value(ptr first365 self) -> i32 { return self.value_field + 2; }
function second365::value(ptr second365 self) -> i32 { return self.value_field + 100; }

function memcpy365(ptr i0 dst, ptr i0 src, i64 count) -> ptr i0 {
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
    @[align(8)] arr first_storage[64, u8];
    @[align(8)] arr second_storage[64, u8];

    ptr first365 first = place(ref first_storage, first365);
    ptr second365 second = place(ref second_storage, second365);

    first.value_field = 40;
    second.value_field = 1;

    memcpy365(ref second_storage, ref first_storage, sizeof(first365));

    ptr first365 copied = (ref second_storage) as ptr first365;
    ptr iface365 item = copied;
    exit item.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

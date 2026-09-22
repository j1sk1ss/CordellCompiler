: BLOCK_TEST :
: RUN_ASM :

interface iface368 {
    @[self] @[abstract]
    function value(ptr iface368 self) -> i32;
}

container first368 implements iface368 {
    i32 value_field;

    @[override] function value(ptr first368 self) -> i32;
}

container second368 implements iface368 {
    i32 value_field;

    @[override] function value(ptr second368 self) -> i32;
}

function first368::value(ptr first368 self) -> i32 { return self.value_field + 100; }
function second368::value(ptr second368 self) -> i32 { return self.value_field + 2; }

start() {
    @[align(8)] arr storage[64, u8];

    ptr first368 first = place(ref storage, first368);
    first.value_field = 1;

    ptr second368 second = place(ref storage, second368);
    second.value_field = 40;

    ptr iface368 item = second;
    exit item.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

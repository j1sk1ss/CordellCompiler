: BLOCK_TEST :
: RUN_ASM :

interface iface340 {
    @[self] @[abstract]
    function value(ptr iface340 self) -> i32;

    @[self] @[abstract]
    function add_other(ptr iface340 self, ptr iface340 other) -> i32;
}
container impl340::iface340 {
    i32 value_field;
    @[override] function value(ptr impl340 self) -> i32;
    @[override] function add_other(ptr impl340 self, ptr iface340 other) -> i32;
}
function impl340::value(ptr impl340 self) -> i32 { return self.value_field; }
function impl340::add_other(ptr impl340 self, ptr iface340 other) -> i32 {
    return self.value_field + other.value();
}

start() {
    impl340 a;
    impl340 b;
    a.value_field = 19;
    b.value_field = 23;
    ptr iface340 base = ref a;
    exit base.add_other(ref b) as u8;
}

:/ OUTPUT
@exit_code=42
/:

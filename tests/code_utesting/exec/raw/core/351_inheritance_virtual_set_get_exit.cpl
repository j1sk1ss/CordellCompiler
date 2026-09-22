: BLOCK_TEST :
: RUN_ASM :

interface iface351 {
    @[self] @[abstract]
    function set(ptr iface351 self, i32 value) -> i0;

    @[self] @[abstract]
    function get(ptr iface351 self) -> i32;
}
container impl351 implements iface351 {
    i32 value_field;
    @[override] function set(ptr impl351 self, i32 value) -> i0;
    @[override] function get(ptr impl351 self) -> i32;
}
function impl351::set(ptr impl351 self, i32 value) -> i0 { self.value_field = value; }
function impl351::get(ptr impl351 self) -> i32 { return self.value_field; }

function run351(ptr iface351 item) -> i32 {
    item.set(42 as i32);
    return item.get();
}

start() {
    impl351 item;
    exit run351(ref item) as u8;
}

:/ OUTPUT
@exit_code=42
/:

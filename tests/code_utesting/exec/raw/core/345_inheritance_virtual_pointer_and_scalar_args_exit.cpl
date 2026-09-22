: BLOCK_TEST :
: RUN_ASM :

interface iface345 {
    @[self] @[abstract]
    function compute(ptr iface345 self, ptr i32 source, i32 add, i32 mul) -> i32;
}
container impl345 implements iface345 {
    @[override]
    function compute(ptr impl345 self, ptr i32 source, i32 add, i32 mul) -> i32;
}
function impl345::compute(ptr impl345 self, ptr i32 source, i32 add, i32 mul) -> i32 {
    return (dref source + add) * mul;
}

start() {
    i32 source = 18;
    impl345 item;
    ptr iface345 base = ref item;
    exit base.compute(ref source, 3 as i32, 2 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

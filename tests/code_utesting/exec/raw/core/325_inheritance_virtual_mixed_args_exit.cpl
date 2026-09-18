: BLOCK_TEST :
: RUN_ASM :

interface iface325 {
    @[self] @[abstract]
    function combine(ptr iface325 self, i32 a, i32 b, i32 c, i32 d) -> i32;
}

container impl325 implements iface325 {
    @[override]
    function combine(ptr impl325 self, i32 a, i32 b, i32 c, i32 d) -> i32;
}

function impl325::combine(ptr impl325 self, i32 a, i32 b, i32 c, i32 d) -> i32 {
    return a + b * 2 + c * 3 + d * 4;
}

start() {
    impl325 item;
    ptr iface325 base = ref item;
    exit base.combine(2 as i32, 3 as i32, 6 as i32, 4 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

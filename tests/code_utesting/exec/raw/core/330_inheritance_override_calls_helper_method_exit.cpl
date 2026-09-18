: BLOCK_TEST :
: RUN_ASM :

interface iface330 {
    @[self] @[abstract]
    function value(ptr iface330 self) -> i32;
}

container impl330 implements iface330 {
    i32 base;

    @[self]
    function helper(ptr impl330 self) -> i32;

    @[override]
    function value(ptr impl330 self) -> i32;
}

function impl330::helper(ptr impl330 self) -> i32 { return self.base + 2; }
function impl330::value(ptr impl330 self) -> i32 { return self.helper(); }

start() {
    impl330 item;
    item.base = 40;
    ptr iface330 base = ref item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

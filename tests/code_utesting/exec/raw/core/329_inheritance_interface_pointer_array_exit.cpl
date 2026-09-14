: BLOCK_TEST :
: RUN_ASM :

interface iface329 {
    @[self]
    function value(ptr iface329 self) -> i32;
}

container first329::iface329 { 
    @[override] function value(ptr first329 self) -> i32; 
}

container second329::iface329 { 
    @[override] function value(ptr second329 self) -> i32; 
}

function first329::value(ptr first329 self) -> i32 { 19 }
function second329::value(ptr second329 self) -> i32 { 23 }

start() {
    first329 a;
    second329 b;
    arr items[2, ptr iface329];
    items[0] = ref a;
    items[1] = ref b;
    exit (items[0].value() + items[1].value()) as u8;
}

:/ OUTPUT
@exit_code=42
/:

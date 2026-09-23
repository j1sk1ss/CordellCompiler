: BLOCK_TEST :
: RUN_ASM :

interface iface379 {
    @[self] @[abstract]
    function value(ptr iface379 self) -> i32;
}

container impl379 implements iface379 {
    i32 value_field;

    @[override]
    function value(ptr impl379 self) -> i32;
}

@[section(".bss")]
glob arr STORAGE379[64, u8];

function impl379::value(ptr impl379 self) -> i32 {
    return self.value_field;
}

start() {
    ptr impl379 item = place(ref STORAGE379, impl379);
    item.value_field = 42;

    ptr iface379 base = item;
    exit base.value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

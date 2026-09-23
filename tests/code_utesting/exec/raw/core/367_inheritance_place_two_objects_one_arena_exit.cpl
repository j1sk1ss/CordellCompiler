: BLOCK_TEST :
: RUN_ASM :

interface iface367 {
    @[self] @[abstract]
    function value(ptr iface367 self) -> i32;
}

container impl367 implements iface367 {
    i32 value_field;

    @[override]
    function value(ptr impl367 self) -> i32;
}

function impl367::value(ptr impl367 self) -> i32 {
    return self.value_field;
}

start() {
    @[align(8)] arr arena[128, u8];
    ptr u8 second_addr = ref arena;
    second_addr = second_addr + 64;

    ptr impl367 first = place(ref arena, impl367);
    ptr impl367 second = place(second_addr, impl367);

    first.value_field = 19;
    second.value_field = 23;

    ptr iface367 left = first;
    ptr iface367 right = second;
    exit (left.value() + right.value()) as u8;
}

:/ OUTPUT
@exit_code=42
/:

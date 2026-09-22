: BLOCK_TEST :
: RUN_ASM :

interface source362 {
    @[self] @[abstract]
    function data(ptr source362 self) -> ptr i32;
}

container impl362 implements source362 {
    ptr i32 value;

    @[override]
    function data(ptr impl362 self) -> ptr i32;
}

function impl362::data(ptr impl362 self) -> ptr i32 {
    return self.value;
}

start() {
    i32 value = 42;
    @[align(8)] arr storage[64, u8];
    ptr impl362 item = place(ref storage, impl362);
    ptr source362 base = item;

    item.value = ref value;
    exit (dref base.data()) as u8;
}

:/ OUTPUT
@exit_code=42
/:

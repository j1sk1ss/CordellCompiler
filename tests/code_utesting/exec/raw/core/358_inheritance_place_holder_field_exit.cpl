: BLOCK_TEST :
: RUN_ASM :

interface value358 {
    @[self] @[abstract]
    function get(ptr value358 self) -> i32;
}

container impl358 implements value358 {
    i32 value;

    @[override]
    function get(ptr impl358 self) -> i32;
}

container holder358 {
    ptr value358 item;
}

function impl358::get(ptr impl358 self) -> i32 {
    return self.value;
}

start() {
    @[align(8)] arr storage[64, u8];
    ptr impl358 item = place(ref storage, impl358);
    holder358 holder;

    item.value = 42;
    holder.item = item;
    exit holder.item.get() as u8;
}

:/ OUTPUT
@exit_code=42
/:

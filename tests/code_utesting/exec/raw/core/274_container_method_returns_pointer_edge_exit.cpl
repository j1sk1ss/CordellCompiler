: BLOCK_TEST :
: RUN_ASM :

container box {
    i32 value;

    @[self]
    function field(ptr box self) -> ptr i32 {
        return ref self.value;
    }
}

start() {
    box b;
    b.value = 1;

    ptr i32 field = b.field();
    dref field = 42;

    exit b.value as u8;
}

:/ OUTPUT
@exit_code=42
/:

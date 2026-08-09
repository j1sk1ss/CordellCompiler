: BLOCK_TEST :
: RUN_ASM :

container cell {
    i32 value;

    @[self]
    function slot(ptr cell self) -> ptr i32 {
        return ref self.value;
    }
}

start() {
    cell c;
    c.value = 5;

    ptr i32 p = c.slot();
    dref p = 42;

    exit c.value as u8;
}

:/ OUTPUT
@exit_code=42
/:

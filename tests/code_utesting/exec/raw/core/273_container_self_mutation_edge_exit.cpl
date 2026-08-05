: BLOCK_TEST :
: RUN_ASM :

container cell {
    i32 value;

    @[self]
    function add(ptr cell self, i32 delta) -> i0 {
        self.value += delta;
    }
}

start() {
    cell c;
    c.value = 10;
    c.add(32 as i32);

    exit c.value as u8;
}

:/ OUTPUT
@exit_code=42
/:

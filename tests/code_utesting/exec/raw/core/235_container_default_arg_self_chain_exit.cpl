: BLOCK_TEST :
: RUN_ASM :

container default_counter {
    i32 value;

    @[self]
    function add(ptr default_counter self, i32 delta = 6 as i32) -> i0 {
        self.value += delta;
    }
}

start() {
    default_counter c;
    c.value = 30;
    c.add();
    c.add();

    exit c.value as u8;
}

:/ OUTPUT
@exit_code=42
/:

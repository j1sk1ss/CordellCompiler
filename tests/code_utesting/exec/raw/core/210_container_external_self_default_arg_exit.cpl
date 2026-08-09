: BLOCK_TEST :
: RUN_ASM :

container accumulator {
    i32 value;

    @[self]
    function add(ptr accumulator self, i32 delta = 5 as i32) -> i0;
}

function accumulator::add(ptr accumulator self, i32 delta = 5 as i32) -> i0 {
    self.value += delta;
}

start() {
    accumulator a;
    a.value = 30;
    a.add();
    a.add(7 as i32);

    exit a.value as u8;
}

:/ OUTPUT
@exit_code=42
/:

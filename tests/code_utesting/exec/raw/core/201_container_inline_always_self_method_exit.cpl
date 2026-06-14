: BLOCK_TEST :
: RUN_ASM :

container inlined_counter {
    i32 value;

    @[inline(always)]
    @[self]
    function bump(ptr inlined_counter self, i32 delta) -> i0 {
        self.value += delta;
    }
}

start() {
    inlined_counter c;
    c.value = 30;
    c.bump(12);

    exit c.value as u8;
}

:/ OUTPUT
@exit_code=42
/:

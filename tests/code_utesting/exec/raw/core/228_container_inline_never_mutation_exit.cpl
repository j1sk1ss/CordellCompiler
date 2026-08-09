: BLOCK_TEST :
: RUN_ASM :

container cold_mutator {
    i32 value;

    @[self]
    function bump(ptr cold_mutator self, i32 delta) -> i0;
}

@[inline(never)]
function cold_mutator::bump(ptr cold_mutator self, i32 delta) -> i0 {
    self.value += delta;
}

start() {
    cold_mutator c;
    c.value = 21;
    c.bump(21);

    exit c.value as u8;
}

:/ OUTPUT
@exit_code=42
/:

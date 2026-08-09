: BLOCK_TEST :
: RUN_ASM :

@[align(8)]
container aligned_cell {
    i8  tag;
    i64 value;
    ptr i32 bonus;

    @[self]
    function score(ptr aligned_cell self) -> i32 {
        return self.tag as i32 + self.value as i32 + dref self.bonus;
    }
}

start() {
    i32 b = 7;
    aligned_cell c;
    c.tag = 5;
    c.value = 30;
    c.bonus = ref b;

    exit c.score() as u8;
}

:/ OUTPUT
@exit_code=42
/:

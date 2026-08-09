: BLOCK_TEST :
: RUN_ASM :

@[align(8)]
container wide_value {
    i8 tag;
    i64 value;

    @[self]
    function score(ptr wide_value self) -> i32 {
        return self.tag as i32 + self.value as i32;
    }
}

container wide_holder {
    wide_value inner;
    i32 bonus;

    @[self]
    function total(ptr wide_holder self) -> i32 {
        return self.inner.score() + self.bonus;
    }
}

start() {
    wide_holder h;
    h.inner.tag = 2;
    h.inner.value = 30;
    h.bonus = 10;

    exit h.total() as u8;
}

:/ OUTPUT
@exit_code=42
/:

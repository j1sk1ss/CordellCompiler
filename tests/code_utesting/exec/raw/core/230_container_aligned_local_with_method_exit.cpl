: BLOCK_TEST :
: RUN_ASM :

container triple_score {
    i32 a;
    i32 b;
    i32 c;

    @[self]
    function sum(ptr triple_score self) -> i32 {
        return self.a + self.b + self.c;
    }
}

start() {
    @[align(16)] triple_score t;
    t.a = 11;
    t.b = 13;
    t.c = 18;

    exit t.sum() as u8;
}

:/ OUTPUT
@exit_code=42
/:

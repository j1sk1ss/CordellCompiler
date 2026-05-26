: RUN_ASM :

container pair {
    i32 a;
    i32 b;

    @[self]
    function sum(ptr pair self) -> i32 {
        return self.a + self.b;
    }
}

start() {
    pair p;
    p.a = 21;
    p.b = 34;

    exit p.sum() as u8;
}

:/ OUTPUT
@exit_code=55
/:

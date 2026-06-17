: BLOCK_TEST :
: RUN_ASM :

container pair {
    i32 a;
    i32 b;

    @[self]
    function swap(ptr pair self) -> i0 {
        i32 tmp = self.a;
        self.a = self.b;
        self.b = tmp;
    }
}

start() {
    pair p;
    p.a = 3;
    p.b = 4;
    p.swap();

    exit (p.a * 10 + p.b) as u8;
}

:/ OUTPUT
@exit_code=43
/:

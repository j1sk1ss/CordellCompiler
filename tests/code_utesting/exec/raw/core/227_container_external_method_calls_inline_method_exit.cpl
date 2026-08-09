: BLOCK_TEST :
: RUN_ASM :

container ext_score {
    i32 a;
    i32 b;

    @[inline(always)]
    @[self]
    function base(ptr ext_score self) -> i32 {
        return self.a + self.b;
    }

    @[self]
    function total(ptr ext_score self) -> i32;
}

function ext_score::total(ptr ext_score self) -> i32 {
    return self.base() + 2;
}

start() {
    ext_score s;
    s.a = 19;
    s.b = 21;

    exit s.total() as u8;
}

:/ OUTPUT
@exit_code=42
/:

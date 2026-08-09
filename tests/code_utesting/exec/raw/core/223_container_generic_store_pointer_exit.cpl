: BLOCK_TEST :
: RUN_ASM :

container generic_writer {
    i32 calls;

    @[self]
    function store<T>(ptr generic_writer self, ptr T p, T value) -> i0 {
        dref p = value;
        self.calls += 1;
    }
}

start() {
    i32 x = 0;
    i8 y = 0;

    generic_writer w;
    w.calls = 0;
    w.store<i32>(ref x, 40 as i32);
    w.store<i8>(ref y, 1 as i8);

    exit (x + y + w.calls) as u8;
}

:/ OUTPUT
@exit_code=43
/:

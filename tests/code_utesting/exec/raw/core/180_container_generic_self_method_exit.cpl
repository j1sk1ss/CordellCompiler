: BLOCK_TEST :
: RUN_ASM :

container slot {
    i32 total;

    @[self]
    function add<T>(ptr slot self, T value) -> i0 {
        self.total += value as i32;
    }
}

start() {
    slot s;
    s.total = 20;
    s.add<i8>(7);
    s.add<u8>(15);

    exit s.total as u8;
}

:/ OUTPUT
@exit_code=42
/:

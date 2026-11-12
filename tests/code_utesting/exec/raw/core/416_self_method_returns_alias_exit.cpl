: BLOCK_TEST :
: RUN_ASM :

container counter416 {
    i32 value;

    @[self]
    function add(ptr counter416 self, i32 delta) -> ptr counter416 {
        self.value += delta;
        return self;
    }
}

start() {
    counter416 counter;
    counter.value = 1;
    ptr counter416 alias = counter.add(20);
    alias.add(21);
    exit counter.value as u8;
}

:/ OUTPUT
@exit_code=42
/:

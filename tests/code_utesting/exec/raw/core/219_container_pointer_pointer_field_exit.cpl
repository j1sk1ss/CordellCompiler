: BLOCK_TEST :
: RUN_ASM :

container deep_writer {
    ptr ptr i32 target;

    @[self]
    function write(ptr deep_writer self, i32 value) -> i0 {
        ptr i32 p = dref self.target;
        dref p = value;
    }
}

start() {
    i32 x = 0;
    ptr i32 p = ref x;

    deep_writer w;
    w.target = ref p;
    w.write(42);

    exit x as u8;
}

:/ OUTPUT
@exit_code=42
/:

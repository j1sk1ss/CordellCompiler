: BLOCK_TEST :
: RUN_ASM :

container writer {
    ptr i32 target;

    @[self]
    function write(ptr writer self, i32 value) -> i0 {
        dref self.target = value;
    }
}

start() {
    i32 x = 0;
    writer w;
    w.target = ref x;
    w.write(42);

    exit x as u8;
}

:/ OUTPUT
@exit_code=42
/:

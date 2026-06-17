: BLOCK_TEST :
: RUN_ASM :
container int_box {
    i32 value;

    @[self]
    function set<T>(ptr int_box self, T x) -> i0 {
        self.value = x as i32;
    }
}

start() {
    arr boxes[3, int_box];
    boxes[0].set<i8>(5 as i8);
    boxes[1].set<u8>(16 as u8);
    boxes[2].set<i32>(21 as i32);

    exit (boxes[0].value + boxes[1].value + boxes[2].value) as u8;
}

:/ OUTPUT
@exit_code=42
/:

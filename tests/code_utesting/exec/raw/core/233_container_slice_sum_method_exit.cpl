: BLOCK_TEST :
: RUN_ASM :

container slice {
    ptr i32 data;
    i32 len;

    @[self]
    function sum(ptr slice self) -> i32 {
        i32 i = 0;
        i32 acc = 0;

        while i < self.len; {
            acc += self.data[i];
            i += 1;
        }

        return acc;
    }
}

start() {
    arr data[5, i32] = { 4, 6, 8, 10, 14 };
    slice s;
    s.data = ref data;
    s.len = 5;

    exit s.sum() as u8;
}

:/ OUTPUT
@exit_code=42
/:

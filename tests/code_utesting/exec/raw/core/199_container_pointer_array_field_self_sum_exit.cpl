: BLOCK_TEST :
: RUN_ASM :

container row_ref {
    ptr i32 data;
    i32 len;

    @[self]
    function sum(ptr row_ref self) -> i32 {
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
    arr row[4, i32] = { 8, 9, 10, 15 };
    row_ref r;
    r.data = ref row;
    r.len = 4;

    exit r.sum() as u8;
}

:/ OUTPUT
@exit_code=42
/:

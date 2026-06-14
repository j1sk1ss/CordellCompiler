: BLOCK_TEST :
: RUN_ASM :

container int_buffer {
    arr values[4, i32];

    @[self]
    function fill(ptr int_buffer self, i32 base) -> i0 {
        i32 i = 0;
        while i < 4; {
            self.values[i] = base + i;
            i += 1;
        }
    }

    @[self]
    function sum(ptr int_buffer self) -> i32 {
        return self.values[0] + self.values[1] + self.values[2] + self.values[3];
    }
}

start() {
    int_buffer b;
    b.fill(9);

    exit b.sum() as u8;
}

:/ OUTPUT
@exit_code=42
/:

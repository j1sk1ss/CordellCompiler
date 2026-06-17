: BLOCK_TEST :
: RUN_ASM :

container ring {
    arr data[4, i32];
    i32 pos;

    @[self]
    function push(ptr ring self, i32 value) -> i0 {
        self.data[self.pos] = value;
        self.pos += 1;
    }

    @[self]
    function sum(ptr ring self) -> i32 {
        return self.data[0] + self.data[1] + self.data[2] + self.data[3];
    }
}

start() {
    ring r;
    r.pos = 0;
    r.push(4);
    r.push(8);
    r.push(12);
    r.push(18);

    exit r.sum() as u8;
}

:/ OUTPUT
@exit_code=42
/:

: RUN_ASM :

container counter {
    i32 value;

    @[self]
    function add(ptr counter self, i32 delta) -> i0 {
        self.value += delta;
    }

    @[self]
    function sub(ptr counter self, i32 delta) -> i0 {
        self.value -= delta;
    }
}

start() {
    counter c;
    c.value = 10;
    c.add(7);
    c.sub(3);

    exit c.value as u8;
}

:/ OUTPUT
@exit_code=14
/:

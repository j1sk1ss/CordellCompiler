: BLOCK_TEST :
: RUN_ASM :

container payload {
    i32 value;
}

container envelope {
    payload body;
    i32 checksum;

    @[self]
    function update(ptr envelope self, i32 delta) -> i0;

    @[self]
    function total(ptr envelope self) -> i32;
}

function envelope::update(ptr envelope self, i32 delta) -> i0 {
    self.body.value += delta;
    self.checksum += delta * 2;
}

function envelope::total(ptr envelope self) -> i32 {
    return self.body.value + self.checksum;
}

start() {
    envelope e;
    e.body.value = 10;
    e.checksum = 20;
    e.update(4);

    exit e.total() as u8;
}

:/ OUTPUT
@exit_code=42
/:

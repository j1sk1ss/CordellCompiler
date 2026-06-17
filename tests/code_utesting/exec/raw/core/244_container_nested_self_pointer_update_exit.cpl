: BLOCK_TEST :
: RUN_ASM :

container payload {
    i32 value;
}

container payload_ref {
    ptr payload target;

    @[self]
    function add(ptr payload_ref self, i32 delta) -> i0 {
        self.target.value += delta;
    }
}

container envelope {
    payload body;
    payload_ref ref_body;

    @[self]
    function prepare(ptr envelope self) -> i0 {
        self.ref_body.target = ref self.body;
    }
}

start() {
    envelope e;
    e.body.value = 30;
    e.prepare();
    e.ref_body.add(12);

    exit e.body.value as u8;
}

:/ OUTPUT
@exit_code=42
/:

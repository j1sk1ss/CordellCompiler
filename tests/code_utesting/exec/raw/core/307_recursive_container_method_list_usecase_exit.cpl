: BLOCK_TEST :
: RUN_ASM :

container mini_node {
    ptr mini_node n;
    ptr mini_node p;
    i32 value;
}

container mini_list {
    ptr mini_node h;
    ptr mini_node t;
    u64 len;

    @[self]
    @[abi]
    function init(ptr mini_list self) -> i0 {
        self.h = 0 as ptr mini_node;
        self.t = 0 as ptr mini_node;
        self.len = 0 as u64;
    }

    @[self]
    @[abi]
    function push_back(ptr mini_list self, ptr mini_node block) -> i0 {
        block.p = self.t;
        block.n = 0 as ptr mini_node;

        if self.t; self.t.n = block;
        else self.h = block;

        self.t = block;
        self.len += 1;
    }

    @[self]
    @[abi]
    function push_front(ptr mini_list self, ptr mini_node block) -> i0 {
        block.n = self.h;
        block.p = 0 as ptr mini_node;

        if self.h; self.h.p = block;
        else self.t = block;

        self.h = block;
        self.len += 1;
    }

    @[self]
    function checksum(ptr mini_list self) -> i32 {
        return self.h.value + self.h.n.value * 2 + self.t.value * 3 + self.len as i32;
    }
}

start() {
    mini_node a;
    mini_node b;
    mini_node c;
    mini_list list;

    a.value = 4;
    b.value = 5;
    c.value = 6;

    list.init();
    list.push_back(ref a);
    list.push_back(ref b);
    list.push_front(ref c);

    exit list.checksum() as u8;
}

:/ OUTPUT
@exit_code=32
/:

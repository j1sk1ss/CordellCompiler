: BLOCK_TEST :
: RUN_ASM :

container node {
    i32 value;
}

container node_refs {
    arr items[3, ptr node];

    @[self]
    function sum(ptr node_refs self) -> i32 {
        return self.items[0].value + self.items[1].value + self.items[2].value;
    }
}

start() {
    node a;
    node b;
    node c;
    a.value = 5;
    b.value = 17;
    c.value = 20;

    node_refs refs;
    refs.items[0] = ref a;
    refs.items[1] = ref b;
    refs.items[2] = ref c;

    exit refs.sum() as u8;
}

:/ OUTPUT
@exit_code=42
/:

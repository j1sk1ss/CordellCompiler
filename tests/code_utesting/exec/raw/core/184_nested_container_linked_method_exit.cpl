: BLOCK_TEST :
: RUN_ASM :

container node {
    ptr node next;
    i32 value;

    @[self]
    function total2(ptr node self) -> i32 {
        return self.value + self.next.value;
    }
}

container graph {
    node head;
    node tail;

    @[self]
    function score(ptr graph self) -> i32 {
        return self.head.total2();
    }
}

start() {
    graph g;
    g.head.value = 18 as i32;
    g.tail.value = 24 as i32;
    g.head.next = ref g.tail;

    exit g.score() as u8;
}

:/ OUTPUT
@exit_code=42
/:

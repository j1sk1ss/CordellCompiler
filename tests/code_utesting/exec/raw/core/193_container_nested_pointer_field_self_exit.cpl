: BLOCK_TEST :
: RUN_ASM :

container chain_node {
    ptr chain_node next;
    i32 value;

    @[self]
    function pair_sum(ptr chain_node self) -> i32 {
        return self.value + self.next.value;
    }
}

container chain_pair {
    chain_node first;
    chain_node second;

    @[self]
    function total(ptr chain_pair self) -> i32 {
        return self.first.pair_sum();
    }
}

start() {
    chain_pair p;
    p.first.value = 17;
    p.second.value = 25;
    p.first.next = ref p.second;

    exit p.total() as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

container node {
    ptr node next;
    i32 value;

    @[self]
    function sum(ptr node self) -> i32 {
        ptr node p = self;
        i32 acc = 0;

        while p; {
            acc += p.value;
            p = p.next;
        }

        return acc;
    }
}

start() {
    node a;
    node b;
    node c;
    a.value = 10;
    b.value = 11;
    c.value = 21;
    a.next = ref b;
    b.next = ref c;
    c.next = 0;

    exit a.sum() as u8;
}

:/ OUTPUT
@exit_code=42
/:

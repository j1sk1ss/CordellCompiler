: BLOCK_TEST :
: RUN_ASM :

container chain_link {
    ptr chain_link n;
    ptr chain_link p;
    i32 value;
}

@[abi]
function unlink_middle(ptr chain_link block) -> i0 {
    if block.p; block.p.n = block.n;
    if block.n; block.n.p = block.p;
}

start() {
    chain_link a;
    chain_link b;
    chain_link c;

    a.value = 10;
    b.value = 20;
    c.value = 30;

    a.p = 0 as ptr chain_link;
    a.n = ref b;
    b.p = ref a;
    b.n = ref c;
    c.p = ref b;
    c.n = 0 as ptr chain_link;

    unlink_middle(ref b);

    i32 total = a.n.value + c.p.value;
    exit total as u8;
}

:/ OUTPUT
@exit_code=40
/:

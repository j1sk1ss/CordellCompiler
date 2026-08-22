: BLOCK_TEST :
: RUN_ASM :

container link_node {
    ptr link_node n;
    ptr link_node p;
    i32 value;
}

start() {
    link_node a;
    link_node b;
    link_node c;

    a.value = 1;
    b.value = 2;
    c.value = 3;

    a.n = ref b;
    a.p = ref c;
    b.p = ref a;
    c.n = ref a;

    i32 total = a.n.value + a.p.value * 2 + b.p.value * 3 + c.n.value * 4;
    exit total as u8;
}

:/ OUTPUT
@exit_code=15
/:

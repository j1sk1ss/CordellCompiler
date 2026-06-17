: BLOCK_TEST :
: RUN_ASM :

container node {
    i32 value;
}

start() {
    arr nodes[3, node];
    nodes[0].value = 5;
    nodes[1].value = 17;
    nodes[2].value = 20;

    ptr node p = ref nodes;
    ptr ptr node pp = ref p;

    exit (p[2].value + pp[0].value + p[1].value) as u8;
}

:/ OUTPUT
@exit_code=42
/:

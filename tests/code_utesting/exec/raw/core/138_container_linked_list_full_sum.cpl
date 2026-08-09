: RUN_ASM :

container node {
    ptr node next;
    i32      value;
}

start() {
    node a;
    node b;
    node c;

    a.value = 4;
    b.value = 8;
    c.value = 15;

    a.next = ref b;
    b.next = ref c;
    c.next = 0;

    ptr node head = ref a;
    i32 sum = 0;

    while head; {
        sum += head.value;
        head = head.next;
    }

    exit sum as u8;
}

:/ OUTPUT
@exit_code=27
/:

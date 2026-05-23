: RUN_ASM :

container node {
    ptr node next;
    i32      val;
}

start() {
    node a;
    node b;
    node c;

    a.val = 1;
    b.val = 2;
    c.val = 3;

    a.next = ref b;
    b.next = ref c;
    c.next = 0;

    i32 ans = 0;
    ptr node head = ref a;
    while head.next; {
        ans += head.val;
        head = head.next;
    }

    exit ans as u8;
}

:/ OUTPUT
@exit_code=3
/:
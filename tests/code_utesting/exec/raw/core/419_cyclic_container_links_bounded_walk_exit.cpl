: BLOCK_TEST :
: RUN_ASM :

container node419 {
    ptr node419 next;
    i32 value;
}

start() {
    node419 a;
    node419 b;
    node419 c;
    a.value = 3;
    b.value = 5;
    c.value = 6;
    a.next = ref b;
    b.next = ref c;
    c.next = ref a;

    ptr node419 current = ref a;
    i32 total = 0;
    i32 i = 0;
    while i < 3; {
        total += current.value;
        current = current.next;
        i += 1;
    }
    exit (total * 3) as u8;
}

:/ OUTPUT
@exit_code=42
/:

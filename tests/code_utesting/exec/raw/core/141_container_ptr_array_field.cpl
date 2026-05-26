: RUN_ASM :

container refs {
    arr items[3, ptr i32];
}

start() {
    i32 a = 6;
    i32 b = 10;
    i32 c = 14;

    refs r;
    r.items[0] = ref a;
    r.items[1] = ref b;
    r.items[2] = ref c;

    dref r.items[1] = 20;

    exit (dref r.items[0] + dref r.items[1] + dref r.items[2]) as u8;
}

:/ OUTPUT
@exit_code=40
/:

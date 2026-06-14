: BLOCK_TEST :
: RUN_ASM :

container triple {
    i32 a;
    i32 b;
    i32 c;
}

start() {
    @[align(16)] triple t;
    t.a = 12;
    t.b = 14;
    t.c = 16;

    exit (t.a + t.b + t.c) as u8;
}

:/ OUTPUT
@exit_code=42
/:

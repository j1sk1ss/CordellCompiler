: ONLY_THIS :
: RUN_ASM :

container storage {
    i32 a;
    i8  b;
    i64 c;
}

start() {
    storage s;
    s.a = 10;
    s.b = 5;
    s.c = 11;
    exit (s.a + s.b + s.c) as u8;
}

:/ OUTPUT
@exit_code=26
/:
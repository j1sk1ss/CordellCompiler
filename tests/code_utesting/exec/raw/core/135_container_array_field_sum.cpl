: RUN_ASM :

container bucket {
    i8 tag;
    arr values[4, i32];
    i8 tail;
}

start() {
    bucket b;

    b.tag = 2;
    b.values[0] = 3;
    b.values[1] = 5;
    b.values[2] = 7;
    b.values[3] = 11;
    b.tail = 13;

    exit (b.tag + b.values[0] + b.values[1] + b.values[2] + b.values[3] + b.tail) as u8;
}

:/ OUTPUT
@exit_code=41
/:

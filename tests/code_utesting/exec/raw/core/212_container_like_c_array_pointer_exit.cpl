: BLOCK_TEST :
: RUN_ASM :

@[like_c]
container c_buffer {
    i8 tag;
    arr data[3, i32];
    ptr i32 extra;
}

start() {
    i32 bonus = 6;
    c_buffer b;
    b.tag = 3;
    b.data[0] = 10;
    b.data[1] = 20;
    b.data[2] = 30;
    b.extra = ref bonus;

    exit (b.tag + b.data[1] + dref b.extra + 13) as u8;
}

:/ OUTPUT
@exit_code=42
/:

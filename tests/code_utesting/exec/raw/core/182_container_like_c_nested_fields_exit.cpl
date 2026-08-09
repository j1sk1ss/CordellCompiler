: BLOCK_TEST :
: RUN_ASM :

@[like_c]
container pair {
    i8 tag;
    i32 value;
}

container wrapper {
    pair p;
    i8 bonus;
}

start() {
    wrapper w;
    w.p.tag = 5;
    w.p.value = 30;
    w.bonus = 7;

    exit (w.p.tag + w.p.value + w.bonus) as u8;
}

:/ OUTPUT
@exit_code=42
/:

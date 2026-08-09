: BLOCK_TEST :
: RUN_ASM :

container point {
    i32 x;
    i32 y;
}

function fill(ptr point p, i32 base) -> i0 {
    p.x = base;
    p.y = base + 1;
}

start() {
    point p;
    fill(ref p, 20);

    exit (p.x + p.y + 1) as u8;
}

:/ OUTPUT
@exit_code=42
/:

: RUN_ASM :

container point {
    i32 x;
    i32 y;
}

container rect {
    point left;
    point right;
}

start() {
    rect r;

    r.left.x = 3;
    r.left.y = 5;
    r.right.x = 7;
    r.right.y = 11;

    exit (r.left.x + r.left.y + r.right.x + r.right.y) as u8;
}

:/ OUTPUT
@exit_code=26
/:

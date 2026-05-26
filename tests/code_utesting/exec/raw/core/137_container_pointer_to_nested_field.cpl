: RUN_ASM :

container point {
    i32 x;
    i32 y;
}

container holder {
    point pos;
    i32 bias;
}

start() {
    holder h;
    ptr point p = ref h.pos;

    p.x = 9;
    p.y = 4;
    h.bias = 6;

    exit (h.pos.x + h.pos.y + h.bias) as u8;
}

:/ OUTPUT
@exit_code=19
/:

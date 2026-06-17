: BLOCK_TEST :
: RUN_ASM :

container point {
    i32 x;
    i32 y;
}

start() {
    arr points[3, point];

    points[0].x = 2;
    points[0].y = 3;
    points[1].x = 5;
    points[1].y = 7;
    points[2].x = 11;
    points[2].y = 13;

    exit (points[0].x + points[1].y + points[2].x) as u8;
}

:/ OUTPUT
@exit_code=20
/:

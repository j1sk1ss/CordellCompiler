: BLOCK_TEST :
: RUN_ASM :

container point2d {
    i32 x;
    i32 y;
}

container point_math {
    function pack(ptr point2d p) -> i32 {
        return p.x * 10 + p.y;
    }
}

start() {
    point2d p;
    p.x = 4;
    p.y = 2;

    point_math math;
    exit math.pack(ref p) as u8;
}

:/ OUTPUT
@exit_code=42
/:

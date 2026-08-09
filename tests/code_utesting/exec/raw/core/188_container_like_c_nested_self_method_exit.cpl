: BLOCK_TEST :
: RUN_ASM :

@[like_c]
container c_point {
    i32 x;
    i32 y;

    @[self]
    function total(ptr c_point self) -> i32 {
        return self.x + self.y;
    }
}

container c_rect {
    c_point left;
    c_point right;

    @[self]
    function perimeter_like(ptr c_rect self) -> i32 {
        return self.left.total() + self.right.total();
    }
}

start() {
    c_rect r;
    r.left.x = 3;
    r.left.y = 4;
    r.right.x = 15;
    r.right.y = 20;

    exit r.perimeter_like() as u8;
}

:/ OUTPUT
@exit_code=42
/:

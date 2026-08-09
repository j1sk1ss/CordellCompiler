: BLOCK_TEST :
: RUN_ASM :

container score_box {
    i32 left;
    i32 right;

    @[self]
    function base(ptr score_box self) -> i32 {
        return self.left + self.right;
    }

    @[self]
    function score(ptr score_box self, i32 bonus) -> i32 {
        return self.base() + bonus;
    }
}

start() {
    score_box b;
    b.left = 15;
    b.right = 20;

    exit b.score(7) as u8;
}

:/ OUTPUT
@exit_code=42
/:

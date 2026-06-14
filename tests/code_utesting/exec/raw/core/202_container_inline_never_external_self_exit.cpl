: BLOCK_TEST :
: RUN_ASM :

container cold_score {
    i32 left;
    i32 right;

    @[self]
    function score(ptr cold_score self) -> i32;
}

@[inline(never)]
function cold_score::score(ptr cold_score self) -> i32 {
    return self.left + self.right;
}

start() {
    cold_score s;
    s.left = 19;
    s.right = 23;

    exit s.score() as u8;
}

:/ OUTPUT
@exit_code=42
/:

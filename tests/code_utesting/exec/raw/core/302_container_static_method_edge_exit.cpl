: BLOCK_TEST :
: RUN_ASM :

container edge_math {
    function choose(i32 left, i32 right) -> i32 {
        if left; {
            return left;
        }

        return right;
    }
}

start() {
    exit edge_math::choose(0 as i32, 42 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

container pair_sum {
    i32 left;
    i32 right;
}

@[abi]
function add2(i32 a, i32 b) -> i32 {
    return a + b;
}

@[abi]
function fill_pair_sum(ptr pair_sum box, i32 left, i32 right) -> i0 {
    box.left = left;
    box.right = right;
}

start() {
    pair_sum box;
    i32 base = 5;
    fill_pair_sum(ref box, add2(base, 12 as i32), add2(20 as i32, base));

    exit (box.left + box.right) as u8;
}

:/ OUTPUT
@exit_code=42
/:

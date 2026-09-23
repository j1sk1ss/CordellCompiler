: BLOCK_TEST :
: RUN_ASM :

container pair_box {
    i32 left;
    i32 right;
}

@[abi]
function fill_pair(ptr pair_box box, i32 left, i32 right) -> i0 {
    box.left = left;
    box.right = right;
}

start() {
    pair_box box;
    fill_pair(ref box, 17 as i32, 25 as i32);

    exit (box.left + box.right) as u8;
}

:/ OUTPUT
@exit_code=42
/:

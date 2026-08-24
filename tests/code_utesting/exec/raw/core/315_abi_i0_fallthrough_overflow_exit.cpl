: BLOCK_TEST :
: RUN_ASM :

container abi_pair {
    i32 left;
    i32 right;
}

@[abi]
function fill_pair7(ptr abi_pair pair, i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g) -> i0 {
    pair.left = a + b + c + d + e + f;
    pair.right = g;
}

start() {
    abi_pair pair;
    fill_pair7(ref pair, 1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 21 as i32);

    exit (pair.left + pair.right) as u8;
}

:/ OUTPUT
@exit_code=42
/:

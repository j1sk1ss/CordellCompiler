: BLOCK_TEST :
: RUN_ASM :

@[abi]
function half7(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g) -> i32 {
    return a + b + c + d + e + f + g;
}

start() {
    i32 left = half7(1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 0 as i32);
    i32 right = half7(1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 0 as i32);

    exit (left + right) as u8;
}

:/ OUTPUT
@exit_code=42
/:

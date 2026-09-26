: BLOCK_TEST :
: RUN_ASM :

@[abi]
function weighted410(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 h, i32 i, i32 j) -> i32 {
    return a + b + c + d + e + f + g + h + i + j;
}

start() {
    exit (weighted410(1, 2, 3, 4, 5, 6, 7, 8, 9, 10) - 13) as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

@[abi]
function sum9(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f, i32 g, i32 h, i32 i) -> i32 {
    return a + b + c + d + e + f + g + h + i;
}

start() {
    exit sum9(1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 6 as i32, 7 as i32, 8 as i32, 6 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

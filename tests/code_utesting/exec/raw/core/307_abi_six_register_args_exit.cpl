: BLOCK_TEST :
: RUN_ASM :

@[abi]
function sum6(i32 a, i32 b, i32 c, i32 d, i32 e, i32 f) -> i32 {
    return a + b + c + d + e + f;
}

start() {
    exit sum6(1 as i32, 2 as i32, 3 as i32, 4 as i32, 5 as i32, 27 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

start() {
    i32 a = -43;
    i32 q = a / 5;
    i32 r = a % 5;
    exit ((0 - q) * 5 + (0 - r)) as u8;
}

:/ OUTPUT
@exit_code=43
/:

: BLOCK_TEST :
: RUN_ASM :

@[inline(always)]
function cube(i32 x) -> i32 {
    return x * x * x;
}

start() {
    exit cube(3) as u8;
}

:/ OUTPUT
@exit_code=27
/:

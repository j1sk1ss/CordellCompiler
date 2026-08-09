: BLOCK_TEST :
: RUN_ASM :

container cast_math {
    function sum_as_i32<T>(T a, T b) -> i32 {
        return a as i32 + b as i32;
    }
}

start() {
    cast_math m;

    exit (m.sum_as_i32<i8>(10 as i8, 11 as i8) + m.sum_as_i32<u8>(9 as u8, 12 as u8)) as u8;
}

:/ OUTPUT
@exit_code=42
/:

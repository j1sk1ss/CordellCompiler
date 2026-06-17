: BLOCK_TEST :
: RUN_ASM :

container math_box {
    function add(i32 a, i32 b) -> i32 {
        return a + b;
    }

    function add(i8 a, i8 b) -> i32 {
        return a as i32 + b as i32 + 10;
    }
}

start() {
    exit (math_box::add(12 as i32, 30 as i32) + math_box::add(1 as i8, 2 as i8)) as u8;
}

:/ OUTPUT
@exit_code=55
/:

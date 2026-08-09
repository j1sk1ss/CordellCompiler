: BLOCK_TEST :
: RUN_ASM :

container math_box {
    function mix(i32 a, i32 b) -> i32;
}

function math_box::mix(i32 a, i32 b) -> i32 {
    return a * 10 + b;
}

start() {
    math_box box;
    exit box.mix(4, 2) as u8;
}

:/ OUTPUT
@exit_code=42
/:

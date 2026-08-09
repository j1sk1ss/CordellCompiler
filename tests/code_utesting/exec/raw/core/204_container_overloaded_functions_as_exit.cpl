: BLOCK_TEST :
: RUN_ASM :

container overload_box {
    function value(i8 x) -> i32 {
        return x as i32 + 1;
    }

    function value(i64 x) -> i32 {
        return x as i32 + 2;
    }
}

start() {
    overload_box b;

    exit (b.value(20 as i8) + b.value(19 as i64)) as u8;
}

:/ OUTPUT
@exit_code=42
/:

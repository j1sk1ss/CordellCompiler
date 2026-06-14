: BLOCK_TEST :
: RUN_ASM :

container default_calc {
    function add(i32 a, i32 b = 20 as i32) -> i32 {
        return a + b;
    }
}

start() {
    default_calc c;

    exit c.add(22 as i32) as u8;
}

:/ OUTPUT
@exit_code=42
/:

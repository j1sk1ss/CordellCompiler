: BLOCK_TEST :
: RUN_ASM :

function add(i32 a, i32 b) -> i32 {
    return a + b;
}

function sub(i32 a, i32 b) -> i32 {
    return a - b;
}

start() {
    ptr i0 fn = add;
    i32 value = fn(20 as i32, 20 as i32) as i32;

    fn = sub;
    value += fn(5 as i32, 3 as i32) as i32;

    exit value as u8;
}

:/ OUTPUT
@exit_code=42
/:

: BLOCK_TEST :
: RUN_ASM :

function id408<T>(T value) -> T { return value; }
function sum408<T, U>(T left, U right) -> i32 {
    return left as i32 + right as i32;
}

start() {
    i8 a = id408<i8>(19);
    u64 b = id408<u64>(23 as u64);
    exit sum408<i8, u64>(a, b) as u8;
}

:/ OUTPUT
@exit_code=42
/:

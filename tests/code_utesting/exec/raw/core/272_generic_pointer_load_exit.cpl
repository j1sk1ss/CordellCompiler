: BLOCK_TEST :
: RUN_ASM :

function load<T>(ptr T value) -> T {
    return dref value;
}

start() {
    i32 answer = 42;

    exit load<i32>(ref answer) as u8;
}

:/ OUTPUT
@exit_code=42
/:

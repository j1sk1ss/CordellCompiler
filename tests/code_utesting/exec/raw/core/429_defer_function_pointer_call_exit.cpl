: BLOCK_TEST :
: RUN_ASM :

function add429(ptr i32 value, i32 delta) -> i0 { dref value += delta; }

function run429(ptr i32 value) -> i0 {
    ptr i0 operation = add429;
    defer operation(value, 22);
    dref value += 20;
}

start() {
    i32 value = 0;
    run429(ref value);
    exit value as u8;
}

:/ OUTPUT
@exit_code=42
/:

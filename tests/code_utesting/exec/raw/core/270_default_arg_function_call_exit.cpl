: BLOCK_TEST :
: RUN_ASM :

function base() -> i32 {
    return 40;
}

function add_default(i32 value = base()) -> i32 {
    return value + 2;
}

start() {
    exit add_default() as u8;
}

:/ OUTPUT
@exit_code=42
/:

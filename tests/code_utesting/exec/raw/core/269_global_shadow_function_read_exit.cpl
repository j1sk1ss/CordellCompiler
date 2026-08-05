: BLOCK_TEST :
: RUN_ASM :

glob i32 value = 40;

function read_global() -> i32 {
    return value;
}

start() {
    i32 value = 2;

    exit (read_global() + value) as u8;
}

:/ OUTPUT
@exit_code=42
/:

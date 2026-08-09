: BLOCK_TEST :
: RUN_ASM :

function value() -> i32 {
    return 1;
}

start() {
    function value() -> i32 {
        return 42;
    }

    exit value() as u8;
}

:/ OUTPUT
@exit_code=42
/:

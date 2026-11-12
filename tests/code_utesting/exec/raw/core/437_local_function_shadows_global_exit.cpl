: BLOCK_TEST :
: RUN_ASM :

function value437() -> i32 { return 1; }

function outer437() -> i32 {
    function value437() -> i32 { return 42; }
    return value437();
}

start() {
    exit outer437() as u8;
}

:/ OUTPUT
@exit_code=42
/:

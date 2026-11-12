: BLOCK_TEST :
: RUN_ASM :

function read401(i32 value) -> i32 { return value + 1; }
function read401(ptr i32 value) -> i32 { return dref value + 2; }

start() {
    i32 x = 19;
    exit (read401(x) + read401(ref x)) as u8;
}

:/ OUTPUT
@exit_code=41
/:

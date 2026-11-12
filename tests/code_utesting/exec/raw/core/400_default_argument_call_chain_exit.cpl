: BLOCK_TEST :
: RUN_ASM :

function seed400() -> i32 { return 7; }
function combine400(i32 a, i32 b = seed400(), i32 c = 5) -> i32 {
    return a * b + c;
}

start() {
    exit combine400(5) as u8;
}

:/ OUTPUT
@exit_code=40
/:

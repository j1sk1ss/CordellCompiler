: BLOCK_TEST :
: RUN_ASM :

function even404(i32 n) -> i32;
function odd404(i32 n) -> i32;

function even404(i32 n) -> i32 {
    if n == 0; { return 1; }
    return odd404(n - 1);
}

function odd404(i32 n) -> i32 {
    if n == 0; { return 0; }
    return even404(n - 1);
}

start() {
    exit (even404(40) * 40 + odd404(7) * 2) as u8;
}

:/ OUTPUT
@exit_code=42
/:

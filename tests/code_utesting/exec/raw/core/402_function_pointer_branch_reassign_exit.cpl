: BLOCK_TEST :
: RUN_ASM :

function low402(i32 x) -> i32 { return x - 20; }
function high402(i32 x) -> i32 { return x + 2; }

start() {
    ptr i0 op = low402;
    i32 flag = 1;
    if flag; { op = high402; }
    exit op(40) as u8;
}

:/ OUTPUT
@exit_code=42
/:

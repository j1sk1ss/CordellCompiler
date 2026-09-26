: BLOCK_TEST :
: RUN_ASM :

function swap407<T>(ptr T left, ptr T right) -> i0 {
    T temporary = dref left;
    dref left = dref right;
    dref right = temporary;
}

start() {
    i32 a = 40;
    i32 b = 2;
    swap407<i32>(ref a, ref b);
    exit (a * 20 + b - 38) as u8;
}

:/ OUTPUT
@exit_code=42
/:

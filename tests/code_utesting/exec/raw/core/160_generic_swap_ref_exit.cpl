: BLOCK_TEST :
: RUN_ASM :

function swap<T>(ptr T a, ptr T b) -> i0 {
    T tmp = dref a;
    dref a = dref b;
    dref b = tmp;
}

start() {
    i32 a = 12;
    i32 b = 30;

    swap<i32>(ref a, ref b);

    exit (a + b) as u8;
}

:/ OUTPUT
@exit_code=42
/:

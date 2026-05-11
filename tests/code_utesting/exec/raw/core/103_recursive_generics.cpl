: RUN_ASM :
function foo<T>(T a) -> T {
    function bar<U, K>(T b, U c) -> U {
        K a = 1;
        a;
    }
    return bar<T, i8>(a, 1) as T;
}

start() {
    exit foo<u8>(1);
}

:/ OUTPUT
@exit_code=1
/:
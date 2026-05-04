: TEST_DEBUG :
function foo<T>(T a, T b) -> T {
    function local() -> i32 {
        return 10;
    }
    return a + b + local();
}

start() {
    exit foo<u8>(10, 10) + foo<i8>(10, 10);
}

:/ OUTPUT
/:
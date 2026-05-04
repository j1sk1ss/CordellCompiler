: TEST_DEBUG :
function foo<T>(T a, T b) -> T {
    T d = 10;
    i32 e = 10;
    function local() -> i32 {
        return 10;
    }
    return d + local();
}

start() {
    exit foo<u8>(10, 10) + foo<i8>(10, 10);
}

:/ OUTPUT
/:
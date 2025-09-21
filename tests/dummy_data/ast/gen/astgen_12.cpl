{
    function factorial(i32 n = 10) => i32 {
        if n <= 1; {
            return 1;
        }

        return n * factorial(n - 1);
    }

    start(i64 argc, ptr u64 argv) {
        i32 res = factorial();
        exit 0;
    }
}
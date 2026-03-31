start() {
    function foo(ptr i32 p) -> i32 {
        return dref p;
    }
    ptr i32 a = 0;
    foo(a);
}

:/ OUTPUT
[WARNING] [3{X}] NULL-dereference error (variable 'p' is NULL)!
/:

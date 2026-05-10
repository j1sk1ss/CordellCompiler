function foo<T>(T a) -> T {
    function bar<U, K>(T b, U c) -> U {
        K a;
    }
    bar<T, i8>(a, 1);
}
start() {
    foo<i32>(1);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            foo3__i32(i8n 1);
        }
    }
    fn foo3__i32(i32 a) -> i32
    {
        i32s %7 = alloc;
        i32s %7 = load_arg();
        {
            use i32s %7;
            bar5__i32__i8(i32s %7, i8n 1);
        }
    }
    goto lb30;
    fn bar5__i32__i8(i32 b, i32 c) -> i32
    {
        i32s %10 = alloc;
        i32s %10 = load_arg();
        i32s %9 = alloc;
        i32s %9 = load_arg();
        {
            i8s %8 = alloc;
        }
    }
    lb30:
}
/:
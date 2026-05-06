function foo<U, T>(T a, U b) -> T {
}
function bar<U, T>(T a) -> U {
    return foo<T, U>(a, a);
}
start() {
    bar<i32, i8>(1);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            bar3__i32__i8(i8n 1);
        }
    }
    fn bar3__i32__i8(i8 a) -> i32
    {
        i8s %3 = alloc;
        i8s %3 = load_arg();
        {
            use i8s %3;
            use i8s %3;
            i32t %6 = foo4__i8__i32(i8s %3, i8s %3);
            return i32t %6;
        }
    }
    fn foo4__i8__i32(i32 a, i8 b) -> i32
    {
        i32s %5 = alloc;
        i32s %5 = load_arg();
        i8s %4 = alloc;
        i8s %4 = load_arg();
        {
        }
    }
}
/:
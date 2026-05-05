function foo<T>(T a) -> T {
}
function bar<T>(T a) -> T {
    return foo<T>(a);
}
start() {
    bar<i32>(1);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            bar3__i32(i8n 1);
        }
    }
    fn bar3__i32(i32 a) -> i32
    {
        i32s %2 = alloc;
        i32s %2 = load_arg();
        {
            use i32s %2;
            i32t %4 = foo4__i32(i32s %2);
            return i32t %4;
        }
    }
    fn foo4__i32(i32 a) -> i32
    {
        i32s %3 = alloc;
        i32s %3 = load_arg();
        {
        }
    }
}
/:
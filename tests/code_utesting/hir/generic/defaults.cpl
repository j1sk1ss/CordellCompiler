function foo<T>(T a = 10) -> T {
    return a;
}

function bar<T>(T a = foo<T>()) -> T {
    return a;
}

start() {
    bar<i32>();
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32t %4 = foo3__i32(i8n 10);
            use i32t %4;
            bar4__i32(i32t %4);
        }
    }
    fn bar4__i32(i32 a) -> i32
    {
        i32s %2 = alloc;
        i32s %2 = load_arg();
        {
            return i32s %2;
        }
    }
    fn foo3__i32(i32 a) -> i32
    {
        i32s %3 = alloc;
        i32s %3 = load_arg();
        {
            return i32s %3;
        }
    }
}
/:
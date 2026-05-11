function foo<T>(T a) -> T {
    function bar<U>(T b, U c) -> U {
        function baz<K>(T d, U k, K l) -> K {
            return 1;
        }
        baz<U>(b, c, 1);
    }
    bar<T>(a, 1);
}
start() {
    foo<i32>(1);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            foo4__i32(i8n 1);
        }
    }
    fn foo4__i32(i32 a) -> i32
    {
        i32s %11 = alloc;
        i32s %11 = load_arg();
        {
            use i32s %11;
            bar8__i32(i32s %11, i8n 1);
        }
    }
    fn bar8__i32(i32 b, i32 c) -> i32
    {
        i32s %16 = alloc;
        i32s %16 = load_arg();
        i32s %15 = alloc;
        i32s %15 = load_arg();
        {
            use i32s %16;
            use i32s %15;
            baz10__i32(i32s %16, i32s %15, i8n 1);
        }
    }
    fn baz10__i32(i32 d, i32 k, i32 l) -> i32
    {
        i32s %19 = alloc;
        i32s %19 = load_arg();
        i32s %18 = alloc;
        i32s %18 = load_arg();
        i32s %17 = alloc;
        i32s %17 = load_arg();
        {
            i32t %20 = i8n 1 as i32;
            return i32t %20;
        }
    }
}
/:
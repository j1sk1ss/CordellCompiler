function bar<T>() -> T;
function foo<T>(T a);
function foo<T>(T a) {
    return bar<T>() + a;
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
    fn foo3__i32( a)
    {
        i32s %2 = alloc;
        i32s %2 = load_arg();
        {
            i32t %3 = bar4__i32();
            i32t %4 = i32t %3 + i32s %2;
            return i32t %4;
        }
    }
}
/:
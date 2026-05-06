function foo<T, U>() -> T {
    T a;
    U b;
    return a + b;
}

start() {
    ptr i0 p = foo<i32, i32>;
    p();
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i0s* %2 = alloc;
            i0t* %5 = &(foo2__i32__i32() -> i32);
            i0s* %2 = i0t* %5;
            i0s* %2();
        }
    }
    fn foo2__i32__i32() -> i32
    {
        {
            i32s %4 = alloc;
            i32s %3 = alloc;
            i32t %6 = i32s %4 + i32s %3;
            return i32t %6;
        }
    }
}
/:
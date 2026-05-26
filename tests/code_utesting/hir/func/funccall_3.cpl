function foo(i32 a) -> i32;
start() {
    ptr i0 a = foo;
    i32 b = a(10) as i32;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i0s* %1 = alloc;
            i0t* %3 = &(foo0(i32 a) -> i32);
            i0s* %1 = i0t* %3;
            i32s %2 = alloc;
            i64t %5 = i0s* %1(i8n 10);
            i32t %4 = i64t %5 as i32;
            i32s %2 = i32t %4;
        }
    }
}
/:
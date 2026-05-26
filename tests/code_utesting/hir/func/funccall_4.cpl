function foo(u32 a);
start() {
    ptr i0 a = foo;
    a(10);
    foo(10);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i0s* %1 = alloc;
            i0t* %2 = &(foo0(u32 a));
            i0s* %1 = i0t* %2;
            i0s* %1(i8n 10);
            foo0(i8n 10);
        }
    }
}
/:
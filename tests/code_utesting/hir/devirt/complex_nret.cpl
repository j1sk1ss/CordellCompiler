function foo() {
    i32 a = 1 as i32;
    i32 b = 1 as i32;
    a + b;
}

start() {
    foo();
}

:/ OUTPUT
{
    fn foo0()
    {
        {
            i32s %0 = alloc;
            i32t %2 = i8n 1 as i32;
            i32s %0 = i32t %2;
            i32s %1 = alloc;
            i32t %3 = i8n 1 as i32;
            i32s %1 = i32t %3;
            i32t %4 = i32s %0 + i32s %1;
            return i32t %4;
        }
    }
    fn _main()
    {
        {
            foo0();
        }
    }
}
/:
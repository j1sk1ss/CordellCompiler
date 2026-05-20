function foo(i32 a) -> i32;
start() {
    foo((10 + 10) + (10 + 10));
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i8t %1 = i8n 10 + i8n 10;
            [unused] i8t %2 = i8n 10 + i8n 10;
            i8t %3 = i8t %1 + i8t %1;
            use i8t %3;
            foo0(i8t %3);
        }
    }
}
/:
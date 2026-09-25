function foo(i32 a) -> i0;

start() {
    defer foo(10);
    foo(10);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            [unused] defer {
                [unused] foo0(i8n 10);
            [unused] }
            foo0(i8n 10);
            foo0(i8n 10);
        }
    }
}
/:
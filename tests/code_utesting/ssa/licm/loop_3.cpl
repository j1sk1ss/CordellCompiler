function foo();
start() {
    loop foo();
}

:/ OUTPUT
{
    fn _main()
    {
        {
            lb8:
            goto lb9;
            lb9:
            {
                foo0();
            }
            goto lb8;
            lb10:
        [unused] }
    [unused] }
    [unused]
}
/:
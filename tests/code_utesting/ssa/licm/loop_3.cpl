function foo();
start() {
    loop foo();
}

:/ OUTPUT
{
    start {
        {
            {
                lb8:
                goto lb9;
                lb9:
                {
                    call foo0(), argc args();
                }
                goto lb8;
                lb10:
            [unused] }
        [unused] }
    [unused] }
}
/:
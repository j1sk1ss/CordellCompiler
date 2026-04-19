function foo();
start() {
    loop foo();
}

:/ OUTPUT
{
    start {
        {
            {
                lb10:
                goto lb11;
                lb11:
                {
                    call foo0(), argc args();
                }
                goto lb10;
                lb12:
            [unused] }
        [unused] }
    [unused] }
}
/:
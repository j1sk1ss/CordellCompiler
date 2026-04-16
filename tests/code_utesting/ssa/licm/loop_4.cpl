function foo(i32 a);
start() {
    loop foo(10 + 10 * 1);
}

:/ OUTPUT
{
    start {
        {
            {
                i8t %1 = i8n 10 * i8n 1;
                i8t %2 = i8n 10 + i8t %1;
                use i8t %2;
                lb10:
                goto lb11;
                lb11:
                {
                    call foo0(i32 a), argc args(i8t %2,);
                }
                goto lb10;
                lb12:
            [unused] }
        [unused] }
    [unused] }
}
/:
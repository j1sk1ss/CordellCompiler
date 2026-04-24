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
                lb8:
                goto lb9;
                lb9:
                {
                    use i8t %2;
                    call foo0(i32 a), argc args(i8t %2,);
                }
                goto lb8;
                lb10:
            [unused] }
        [unused] }
    [unused] }
}
/:
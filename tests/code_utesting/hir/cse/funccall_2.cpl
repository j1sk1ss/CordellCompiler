function foo(i32 a, i32 b) -> i32;
start() {
    foo(100 + 1, 100 + 1);
}

:/ OUTPUT
{
    start {
        {
            {
                i8t %2 = i8n 100 + i8n 1;
                use i8t %2;
                [unused] i8t %3 = i8n 100 + i8n 1;
                use i8t %2;
                call foo0(i32 a, i32 b) -> i32, argc args(i8t %2,i8t %2,);
            }
        }
    }
}
/:
function foo() {
    10;
}
start() {
    exit foo();
}

:/ OUTPUT
{
    fn foo0()
    {
        {
            return i8n 10;
        }
    }
    start {
        {
            {
                i64t %0 = foo0();
                exit i64t %0;
            }
        }
    }
}
/:
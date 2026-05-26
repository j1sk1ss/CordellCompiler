function foo() {
    10;
}
start() {
    exit foo() as u8;
}

:/ OUTPUT
{
    fn foo0()
    {
        {
            return i8n 10;
        }
    }
    fn _main()
    {
        {
            i64t %1 = foo0();
            u8t %0 = i64t %1 as u8;
            exit u8t %0;
        }
    }
}
/:
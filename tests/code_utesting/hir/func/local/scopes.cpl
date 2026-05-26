{
    function foo() {
        return 1;
    }
}

function foo() {
    return 2;
}

start() {
    exit foo() as u8;
}

:/ OUTPUT
{
    {
        fn foo0()
        {
            {
                return i8n 1;
            }
        }
    }
    fn foo1()
    {
        {
            return i8n 2;
        }
    }
    fn _main()
    {
        {
            i64t %1 = foo1();
            u8t %0 = i64t %1 as u8;
            exit u8t %0;
        }
    }
}
/:
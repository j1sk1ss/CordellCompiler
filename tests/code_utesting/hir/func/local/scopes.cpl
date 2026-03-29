{
    function foo() {
        return 1;
    }
}

function foo() {
    return 2;
}

start() {
    exit foo();
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
    start {
        {
            {
                i64t %0 = call foo1(), argc args();
                exit i64t %0;
            }
        }
    }
}
/:

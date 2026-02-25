{
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
}

: OUTPUT
{
    {
        fn foo0()
        {
            {
                return num? 1;
            }
        }
    }
    fn foo1()
    {
        {
            return num? 2;
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
:
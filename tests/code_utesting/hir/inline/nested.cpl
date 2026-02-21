{
    function foo() {
        return 1;
    }

    function bar() {
        return foo();
    }

    function baz() {
        return bar();
    }

    start() {
        exit baz();
    }
}

: OUTPUT
{
    fn foo0()
    {
        {
            return num? 1;
        }
    }
    fn bar1()
    {
        {
            i64t %0 = num? 1;
            [unused] i64t %0 = call foo0(), argc args();
            return i64t %0;
        }
    }
    fn baz2()
    {
        {
            i64t %0 = num? 1;
            i64t %1 = i64t %0;
            [unused] i64t %1 = call bar1(), argc args();
            return i64t %1;
        }
    }
    start {
        {
            {
                i64t %0 = num? 1;
                i64t %1 = i64t %0;
                i64t %2 = i64t %1;
                [unused] i64t %2 = call baz2(), argc args();
                exit i64t %2;
            }
        }
    }
}
:
{
    function foo() {
        return 100;
    }
    start() {
        while 1; {
            while 1; {
                foo();
                break;
            }
        }
    }
}

: OUTPUT
{
    fn foo0()
    {
        {
            return num? 100;
        }
    }
    start {
        {
            {
                lb10:
                if num? 1, goto lb11, else goto lb12;
                lb11:
                {
                    lb15:
                    if num? 1, goto lb16, else goto lb17;
                    lb16:
                    {
                        [unused] call foo0(), argc args();
                        // break;
                        goto lb17;
                    }
                    goto lb15;
                    lb17:
                }
                goto lb10;
                lb12:
            }
        }
    }
}
:
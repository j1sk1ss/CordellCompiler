{
    function getter() {
        return 100;
    }

    start() {
        while 1; {
            while 1; {
                getter();
            }
        }
    }
}

: OUTPUT
{
    fn getter0()
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
                        [unused] call getter0(), argc args();
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
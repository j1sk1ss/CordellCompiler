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
                lb{X}
                if num? 1, goto lb{X} else goto lb{X}
                lb{X}
                {
                    lb{X}
                    if num? 1, goto lb{X} else goto lb{X}
                    lb{X}
                    {
                        [unused] call foo0(), argc args();
                        // break;
                        goto lb{X}
                    [unused] }
                    [unused] goto lb{X}
                    lb{X}
                }
                goto lb{X}
                lb{X}
            }
        }
    }
}
:
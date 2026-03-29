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

:/ OUTPUT
{
    fn foo0()
    {
        {
            return i8n 100;
        }
    }
    start {
        {
            {
                lb21:
                if i8n 1, goto lb22, else goto lb23;
                lb22:
                {
                    lb29:
                    if i8n 1, goto lb30, else goto lb31;
                    lb30:
                    {
                        [unused] call foo0(), argc args();
                        // break;
                        goto lb31;
                    [unused] }
                    [unused] goto lb29;
                    lb31:
                }
                goto lb21;
                lb23:
            }
        }
    }
}
/:
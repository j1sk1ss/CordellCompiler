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
                lb15:
                if i8n 1, goto lb16, else goto lb17;
                lb16:
                {
                    lb21:
                    if i8n 1, goto lb22, else goto lb23;
                    lb22:
                    {
                        lb35:
                        [unused] foo0();
                        // break;
                        goto lb23;
                    }
                    goto lb21;
                    lb23:
                }
                goto lb15;
                lb17:
            }
        }
    }
}
/:
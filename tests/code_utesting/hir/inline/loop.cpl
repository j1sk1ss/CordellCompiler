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

:/ OUTPUT
{
    fn getter0()
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
                        [unused] call getter0(), argc args();
                    }
                    goto lb29;
                    lb31:
                }
                goto lb21;
                lb23:
            }
        }
    }
}
/:
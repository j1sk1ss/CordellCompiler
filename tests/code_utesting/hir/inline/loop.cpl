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
                        [unused] call getter0(), argc args();
                    }
                    goto lb{X}
                    lb{X}
                }
                goto lb{X}
                lb{X}
            }
        }
    }
}
/:

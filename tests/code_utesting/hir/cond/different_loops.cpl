start() {
    loop {
        @[counter(5)] loop {
            @[counter(1)] loop {
            }
        }
    }
}

:/ OUTPUT
{
    start {
        {
            {
                lb{X}
                goto lb{X}
                lb{X}
                {
                    i64s %0 = alloc(8);
                    i64s %0 = cnst? 5;
                    lb{X}
                    goto lb{X}
                    lb{X}
                    {
                        i64s %1 = alloc(8);
                        i64s %1 = cnst? 1;
                        lb{X}
                        goto lb{X}
                        lb{X}
                        {
                            i64t %2 = i64s %1 - cnst? 1;
                            i64s %1 = i64t %2;
                            if i64t %2, goto lb{X} else goto lb{X}
                        }
                        lb{X}
                        i64t %3 = i64s %0 - cnst? 1;
                        i64s %0 = i64t %3;
                        if i64t %3, goto lb{X} else goto lb{X}
                    }
                    lb{X}
                }
                goto lb{X}
                lb{X}
            }
        }
    }
}
/:

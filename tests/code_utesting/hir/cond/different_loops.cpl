{
    start() {
        loop {
            @[counter(5)] loop {
                @[counter(1)] loop {
                }
            }
        }
    }
}

: OUTPUT
{
    start {
        {
            {
                lb4:
                goto lb5;
                lb5:
                {
                    i64s %0 = alloc(8);
                    i64s %0 = cnst? 5;
                    lb10:
                    goto lb11;
                    lb11:
                    {
                        i64s %1 = alloc(8);
                        i64s %1 = cnst? 1;
                        lb16:
                        goto lb17;
                        lb17:
                        {
                            i64s %1 = i64s %1 - cnst? 1;
                            if i64s %1, goto lb16, else goto lb18;
                        }
                        lb18:
                        i64s %0 = i64s %0 - cnst? 1;
                        if i64s %0, goto lb10, else goto lb12;
                    }
                    lb12:
                }
                goto lb4;
                lb6:
            }
        }
    }
}
:
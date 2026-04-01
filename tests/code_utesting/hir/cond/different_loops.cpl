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
                lb10:
                goto lb11;
                lb11:
                {
                    i64s %0 = alloc(8);
                    i64s %0 = cnst? 5;
                    lb18:
                    goto lb19;
                    lb19:
                    {
                        i64s %1 = alloc(8);
                        i64s %1 = cnst? 1;
                        lb26:
                        goto lb27;
                        lb27:
                        {
                            i64t %2 = i64s %1 - cnst? 1;
                            i64s %1 = i64t %2;
                            if i64s %1, goto lb26, else goto lb28;
                        }
                        lb28:
                        i64t %3 = i64s %0 - cnst? 1;
                        i64s %0 = i64t %3;
                        if i64s %0, goto lb18, else goto lb20;
                    }
                    lb20:
                }
                goto lb10;
                lb12:
            }
        }
    }
}
/:
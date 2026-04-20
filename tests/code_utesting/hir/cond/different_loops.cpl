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
                lb7:
                goto lb8;
                lb8:
                {
                    i64s %0 = alloc(8);
                    i64s %0 = csu8 5;
                    lb14:
                    goto lb15;
                    lb15:
                    {
                        i64s %1 = alloc(8);
                        i64s %1 = csu8 1;
                        lb21:
                        goto lb22;
                        lb22:
                        {
                        }
                        i64t %2 = i64s %1 - csu8 1;
                        i64s %1 = i64t %2;
                        if i64s %1, goto lb21, else goto lb23;
                        lb23:
                    }
                    i64t %3 = i64s %0 - csu8 1;
                    i64s %0 = i64t %3;
                    if i64s %0, goto lb14, else goto lb16;
                    lb16:
                }
                goto lb7;
                lb9:
            }
        }
    }
}
/:
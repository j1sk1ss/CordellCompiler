start() {
    @[counter(10)] loop {
    }
}

: OUTPUT
{
    start {
        {
            {
                i64s %0 = alloc(8);
                i64s %0 = cnst? 10;
                lb{X}
                goto lb{X}
                lb{X}
                {
                    i64t %1 = i64s %0 - cnst? 1;
                    i64s %0 = i64t %1;
                    if i64t %1, goto lb{X} else goto lb{X}
                }
                lb{X}
            }
        }
    }
}
:
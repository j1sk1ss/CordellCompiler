start() {
    @[no_fall]
    switch 1; {
        @[cold] case 1; { exit 1; }
        default { exit 2; }
    }
}

: OUTPUT
{
    start {
        {
            {
                goto lb{X}
                lb{X}
                {
                    exit num? 2;
                }
                goto lb{X}
                goto lb{X}
                lb{X}
                i8t %0 = num? 1 < cnst? 1;
                if i8t %0, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                i8t %1 = num? 1 > cnst? 1;
                if i8t %1, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
            }
            lb{X}
            {
                exit num? 1;
            }
            goto lb{X}
        }
    }
}
:
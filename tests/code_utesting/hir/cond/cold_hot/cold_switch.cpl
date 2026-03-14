start() {
    @[no_fall]
    switch 1; {
        @[cold] case 1; { exit 1; }
        @[cold] case 2; { exit 2; }
        case 3; { exit 3; }
        default { exit 4; }
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
                    exit num? 3;
                }
                goto lb{X}
                lb{X}
                {
                    exit num? 4;
                }
                goto lb{X}
                goto lb{X}
                lb{X}
                i8t %0 = num? 1 < cnst? 2;
                if i8t %0, goto lb{X} else goto lb{X}
                lb{X}
                i8t %1 = num? 1 < cnst? 1;
                if i8t %1, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                i8t %2 = num? 1 > cnst? 1;
                if i8t %2, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                i8t %3 = num? 1 > cnst? 2;
                if i8t %3, goto lb{X} else goto lb{X}
                lb{X}
                i8t %4 = num? 1 < cnst? 3;
                if i8t %4, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                i8t %5 = num? 1 > cnst? 3;
                if i8t %5, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
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
            lb{X}
            {
                exit num? 2;
            }
            goto lb{X}
        }
    }
}
:
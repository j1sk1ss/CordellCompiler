start() {
    switch 1; {
        case 1; {}
        case 2; {}
        case 3; {}
        default {}
    }

    exit 1;
}

: OUTPUT
setpos, line=1, column=7, file=<unknown>
{
setpos, line=1, column=7, file=<unknown>
setpos, line=1, column=7, file=<unknown>
    start {
        {
setpos, line=2, column=12, file=<unknown>
            {
setpos, line=2, column=12, file=<unknown>
setpos, line=2, column=12, file=<unknown>
                goto lb10;
                lb12:
setpos, line=3, column=24, file=<unknown>
                {
                }
                lb16:
setpos, line=4, column=24, file=<unknown>
                {
                }
                lb20:
setpos, line=5, column=24, file=<unknown>
                {
                }
                lb24:
setpos, line=6, column=22, file=<unknown>
                {
                }
                goto lb11;
                lb10:
setpos, line=2, column=15, file=<unknown>
                i8t %0 = num? 1 < cnst? 2;
                if i8t %0, goto lb30, else goto lb31;
                lb30:
                i8t %1 = num? 1 < cnst? 1;
                if i8t %1, goto lb36, else goto lb37;
                lb36:
                goto lb24;
                lb37:
                i8t %2 = num? 1 > cnst? 1;
                if i8t %2, goto lb38, else goto lb39;
                lb38:
                goto lb24;
                lb39:
                goto lb12;
                lb31:
                i8t %3 = num? 1 > cnst? 2;
                if i8t %3, goto lb32, else goto lb33;
                lb32:
                i8t %4 = num? 1 < cnst? 3;
                if i8t %4, goto lb46, else goto lb47;
                lb46:
                goto lb24;
                lb47:
                i8t %5 = num? 1 > cnst? 3;
                if i8t %5, goto lb48, else goto lb49;
                lb48:
                goto lb24;
                lb49:
                goto lb20;
                lb33:
                goto lb16;
                lb11:
setpos, line=9, column=10, file=<unknown>
setpos, line=9, column=13, file=<unknown>
                exit num? 1;
            }
        }
    }
}
:
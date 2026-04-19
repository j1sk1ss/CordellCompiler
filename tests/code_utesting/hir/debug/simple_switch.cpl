start() {
    switch 1; {
        case 1; {}
        case 2; {}
        case 3; {}
        default {}
    }

    exit 1;
}

:/ OUTPUT
setpos, line=1, column=7, file={X}
{
setpos, line=1, column=7, file={X}
setpos, line=1, column=7, file={X}
    start {
        {
setpos, line=2, column=12, file={X}
            {
setpos, line=2, column=12, file={X}
setpos, line=2, column=12, file={X}
                goto lb10;
                lb12:
setpos, line=3, column=24, file={X}
                {
                }
                lb16:
setpos, line=4, column=24, file={X}
                {
                }
                lb20:
setpos, line=5, column=24, file={X}
                {
                }
                lb24:
setpos, line=6, column=22, file={X}
                {
                }
                goto lb11;
                lb10:
setpos, line=2, column=15, file={X}
                i64t %1 = i8n 1;
                i8t %0 = i64t %1 < cnst? 2;
                if i8t %0, goto lb30, else goto lb31;
                lb30:
                i64t %3 = i8n 1;
                i8t %2 = i64t %3 < cnst? 1;
                if i8t %2, goto lb37, else goto lb38;
                lb37:
                goto lb24;
                lb38:
                i64t %5 = i8n 1;
                i8t %4 = i64t %5 > cnst? 1;
                if i8t %4, goto lb39, else goto lb40;
                lb39:
                goto lb24;
                lb40:
                goto lb12;
                lb31:
                i64t %7 = i8n 1;
                i8t %6 = i64t %7 > cnst? 2;
                if i8t %6, goto lb32, else goto lb33;
                lb32:
                i64t %9 = i8n 1;
                i8t %8 = i64t %9 < cnst? 3;
                if i8t %8, goto lb50, else goto lb51;
                lb50:
                goto lb24;
                lb51:
                i64t %11 = i8n 1;
                i8t %10 = i64t %11 > cnst? 3;
                if i8t %10, goto lb52, else goto lb53;
                lb52:
                goto lb24;
                lb53:
                goto lb20;
                lb33:
                goto lb16;
                lb11:
setpos, line=9, column=10, file={X}
setpos, line=9, column=13, file={X}
                exit i8n 1;
            }
        }
    }
}
/:
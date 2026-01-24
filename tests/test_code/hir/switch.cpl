{
    start() {
        switch 1; {
            case 1; {
            }
            case 2; {
                break;
            }
            default {
            }
        }
    }
}

: OUTPUT
{
    start {
        {
            {
                goto lb4;
                lb6:
                {
                }
                lb9:
                {
                    goto lb5;
                }
                lb12:
                {
                }
                goto lb5;
                lb4:
                i8t %0 = num? 1 < cnst? 1;
                if i8t %0, goto lb16, else goto lb17;
                lb16:
                goto lb12;
                lb17:
                i8t %1 = num? 1 > cnst? 1;
                if i8t %1, goto lb18, else goto lb19;
                lb18:
                i8t %2 = num? 1 < cnst? 2;
                if i8t %2, goto lb24, else goto lb25;
                lb24:
                goto lb12;
                lb25:
                i8t %3 = num? 1 > cnst? 2;
                if i8t %3, goto lb26, else goto lb27;
                lb26:
                goto lb12;
                lb27:
                goto lb9;
                lb19:
                goto lb6;
                lb5:
            }
        }
    }
}
:
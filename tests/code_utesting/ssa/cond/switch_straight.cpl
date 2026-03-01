{
    start() {
        @[straight] switch 1; {
            case 1; {}
            case 2; {}
            default {}
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
                }
                lb12:
                {
                }
                goto lb5;
                lb4:
                i8t %0 = num? 1 == cnst? 1;
                if i8t %0, goto lb6, else goto lb16;
                lb16:
                i8t %1 = num? 1 == cnst? 2;
                if i8t %1, goto lb9, else goto lb19;
                lb19:
                goto lb12;
                lb5:
            }
        }
    }
}
:
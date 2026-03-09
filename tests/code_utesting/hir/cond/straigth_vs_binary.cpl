{
    start() {
        switch 1; {
            case 1; {}
            default {}
        }

        @[straight] switch 1; {
            case 1; {}
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
                goto lb5;
                lb4:
                i8t %0 = num? 1 < cnst? 1;
                if i8t %0, goto lb13, else goto lb14;
                lb13:
                goto lb9;
                lb14:
                i8t %1 = num? 1 > cnst? 1;
                if i8t %1, goto lb15, else goto lb16;
                lb15:
                goto lb9;
                lb16:
                goto lb6;
                lb5:
                goto lb21;
                lb23:
                {
                }
                lb26:
                {
                }
                goto lb22;
                lb21:
                i8t %2 = num? 1 == cnst? 1;
                if i8t %2, goto lb23, else goto lb30;
                lb30:
                goto lb26;
                lb22:
            }
        }
    }
}
:
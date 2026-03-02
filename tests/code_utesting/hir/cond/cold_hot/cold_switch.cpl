{
    start() {
        @[no_fall]
        switch 1; {
            @[cold] case 1; { exit 1; }
            @[cold] case 2; { exit 2; }
            case 3; { exit 3; }
            default { exit 4; }
        }
    }
}

: OUTPUT
{
    start {
        {
            {
                goto lb4;
                lb14:
                {
                    exit num? 3;
                }
                goto lb5;
                lb18:
                {
                    exit num? 4;
                }
                goto lb5;
                goto lb5;
                lb4:
                i8t %0 = num? 1 < cnst? 2;
                if i8t %0, goto lb23, else goto lb24;
                lb23:
                i8t %1 = num? 1 < cnst? 1;
                if i8t %1, goto lb29, else goto lb30;
                lb29:
                goto lb18;
                lb30:
                i8t %2 = num? 1 > cnst? 1;
                if i8t %2, goto lb31, else goto lb32;
                lb31:
                goto lb18;
                lb32:
                goto lb6;
                lb24:
                i8t %3 = num? 1 > cnst? 2;
                if i8t %3, goto lb25, else goto lb26;
                lb25:
                i8t %4 = num? 1 < cnst? 3;
                if i8t %4, goto lb39, else goto lb40;
                lb39:
                goto lb18;
                lb40:
                i8t %5 = num? 1 > cnst? 3;
                if i8t %5, goto lb41, else goto lb42;
                lb41:
                goto lb18;
                lb42:
                goto lb14;
                lb26:
                goto lb10;
                lb5:
            }
            lb6:
            {
                exit num? 1;
            }
            goto lb5;
            lb10:
            {
                exit num? 2;
            }
            goto lb5;
        }
    }
}
:
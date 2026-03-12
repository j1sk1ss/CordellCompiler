start() {
    @[no_fall]
    @[straight]
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
                i8t %0 = num? 1 == cnst? 1;
                if i8t %0, goto lb6, else goto lb23;
                lb23:
                i8t %1 = num? 1 == cnst? 2;
                if i8t %1, goto lb10, else goto lb26;
                lb26:
                i8t %2 = num? 1 == cnst? 3;
                if i8t %2, goto lb14, else goto lb29;
                lb29:
                goto lb18;
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
{
    start() {
        @[no_fall]
        switch 1; {
            @[cold] case 1; { exit 1; }
            default { exit 2; }
        }
    }
}

: OUTPUT
{
    start {
        {
            {
                goto lb4;
                lb10:
                {
                    exit num? 2;
                }
                goto lb5;
                goto lb5;
                lb4:
                i8t %0 = num? 1 < cnst? 1;
                if i8t %0, goto lb15, else goto lb16;
                lb15:
                goto lb10;
                lb16:
                i8t %1 = num? 1 > cnst? 1;
                if i8t %1, goto lb17, else goto lb18;
                lb17:
                goto lb10;
                lb18:
                goto lb6;
                lb5:
            }
            lb6:
            {
                exit num? 1;
            }
            goto lb5;
        }
    }
}
:
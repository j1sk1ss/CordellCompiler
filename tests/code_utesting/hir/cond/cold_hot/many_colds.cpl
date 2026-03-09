{
    start() {
        @[cold] if 1; {
            exit 1;
        }
        else {
            exit 2;
        }

        @[cold] if 2; {
            exit 1;
        }
        else {
            exit 2;
        }

        @[cold] if 3; {
            exit 1;
        }
        else {
            exit 2;
        }
    }
}

: OUTPUT
{
    start {
        {
            {
                if num? 1, goto lb4, else goto lb5;
                lb5:
                {
                    exit num? 2;
                }
                goto lb6;
                lb6:
                if num? 2, goto lb14, else goto lb15;
                lb15:
                {
                    exit num? 2;
                }
                goto lb16;
                lb16:
                if num? 3, goto lb24, else goto lb25;
                lb25:
                {
                    exit num? 2;
                }
                goto lb26;
                lb26:
            }
            lb4:
            {
                exit num? 1;
            }
            goto lb6;
            lb14:
            {
                exit num? 1;
            }
            goto lb16;
            lb24:
            {
                exit num? 1;
            }
            goto lb26;
        }
    }
}
:
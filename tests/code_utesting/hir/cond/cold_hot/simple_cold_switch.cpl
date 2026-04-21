start() {
    @[no_fall]
    switch 1; {
        @[cold] case 1; { exit 1; }
        default { exit 2; }
    }
}

:/ OUTPUT
{
    start {
        {
            {
                goto lb8;
                lb15:
                {
                    exit i8n 2;
                }
                goto lb9;
                goto lb9;
                lb8:
                i64t %0 = csu8 1 as i64;
                i64t %2 = i8n 1 as i64;
                i8t %1 = i64t %2 < i64t %0;
                if i8t %1, goto lb22, else goto lb23;
                lb22:
                goto lb15;
                lb23:
                i64t %4 = i8n 1 as i64;
                i8t %3 = i64t %4 > i64t %0;
                if i8t %3, goto lb24, else goto lb25;
                lb24:
                goto lb15;
                lb25:
                goto lb10;
                lb9:
            }
            exit csu8 0;
            lb10:
            {
                exit i8n 1;
            }
            goto lb9;
        }
    }
}
/:
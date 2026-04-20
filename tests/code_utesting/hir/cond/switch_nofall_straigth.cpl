start() {
    i32 a;
    @[no_fall]
    @[straight]
    switch a; {
        case 1; {}
        case 2; {}
        default {}
    }
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                goto lb9;
                lb11:
                {
                }
                goto lb10;
                lb15:
                {
                }
                goto lb10;
                lb19:
                {
                }
                goto lb10;
                goto lb10;
                lb9:
                i64t %2 = i32s %0;
                i8t %1 = i64t %2 == cnst? 1;
                if i8t %1, goto lb11, else goto lb25;
                lb25:
                i64t %4 = i32s %0;
                i8t %3 = i64t %4 == cnst? 2;
                if i8t %3, goto lb15, else goto lb29;
                lb29:
                goto lb19;
                lb10:
            }
        }
    }
}
/:
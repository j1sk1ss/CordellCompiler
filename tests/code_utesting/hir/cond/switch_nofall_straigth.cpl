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
                goto lb12;
                lb14:
                {
                }
                goto lb13;
                lb18:
                {
                }
                goto lb13;
                lb22:
                {
                }
                goto lb13;
                goto lb13;
                lb12:
                i64t %2 = i32s %0;
                i8t %1 = i64t %2 == cnst? 1;
                if i8t %1, goto lb14, else goto lb28;
                lb28:
                i64t %4 = i32s %0;
                i8t %3 = i64t %4 == cnst? 2;
                if i8t %3, goto lb18, else goto lb32;
                lb32:
                goto lb22;
                lb13:
            }
        }
    }
}
/:
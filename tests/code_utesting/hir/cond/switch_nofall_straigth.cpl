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

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                goto lb5;
                lb7:
                {
                }
                goto lb6;
                lb10:
                {
                }
                goto lb6;
                lb13:
                {
                }
                goto lb6;
                goto lb6;
                lb5:
                i8t %1 = i32s %0 == cnst? 1;
                if i8t %1, goto lb7, else goto lb17;
                lb17:
                i8t %2 = i32s %0 == cnst? 2;
                if i8t %2, goto lb10, else goto lb20;
                lb20:
                goto lb13;
                lb6:
            }
        }
    }
}
:
start() {
    i32 a;
    @[straight] switch a; {
        case 1; {}
        case 2; {}
        case 3; {}
        case 4; {}
        default {}
    }
    exit 1;
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
                lb10:
                {
                }
                lb13:
                {
                }
                lb16:
                {
                }
                lb19:
                {
                }
                goto lb6;
                lb5:
                i8t %1 = i32s %0 == cnst? 1;
                if i8t %1, goto lb7, else goto lb23;
                lb23:
                i8t %2 = i32s %0 == cnst? 2;
                if i8t %2, goto lb10, else goto lb26;
                lb26:
                i8t %3 = i32s %0 == cnst? 3;
                if i8t %3, goto lb13, else goto lb29;
                lb29:
                i8t %4 = i32s %0 == cnst? 4;
                if i8t %4, goto lb16, else goto lb32;
                lb32:
                goto lb19;
                lb6:
                exit num? 1;
            }
        }
    }
}
:
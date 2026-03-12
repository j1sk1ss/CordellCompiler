start() {
    i32 a;
    @[no_fall] switch a; {
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
                goto lb6;
                lb10:
                {
                }
                goto lb6;
                lb13:
                {
                }
                goto lb6;
                lb16:
                {
                }
                goto lb6;
                lb19:
                {
                }
                goto lb6;
                goto lb6;
                lb5:
                i32t %2 = cnst? 2 as i32;
                i8t %1 = i32s %0 < i32t %2;
                if i8t %1, goto lb23, else goto lb24;
                lb23:
                i32t %4 = cnst? 1 as i32;
                i8t %3 = i32s %0 < i32t %4;
                if i8t %3, goto lb30, else goto lb31;
                lb30:
                goto lb19;
                lb31:
                i32t %6 = cnst? 1 as i32;
                i8t %5 = i32s %0 > i32t %6;
                if i8t %5, goto lb32, else goto lb33;
                lb32:
                goto lb19;
                lb33:
                goto lb7;
                lb24:
                i32t %8 = cnst? 2 as i32;
                i8t %7 = i32s %0 > i32t %8;
                if i8t %7, goto lb25, else goto lb26;
                lb25:
                i32t %10 = cnst? 3 as i32;
                i8t %9 = i32s %0 < i32t %10;
                if i8t %9, goto lb43, else goto lb44;
                lb43:
                goto lb19;
                lb44:
                i32t %12 = cnst? 3 as i32;
                i8t %11 = i32s %0 > i32t %12;
                if i8t %11, goto lb45, else goto lb46;
                lb45:
                i32t %14 = cnst? 4 as i32;
                i8t %13 = i32s %0 < i32t %14;
                if i8t %13, goto lb53, else goto lb54;
                lb53:
                goto lb19;
                lb54:
                i32t %16 = cnst? 4 as i32;
                i8t %15 = i32s %0 > i32t %16;
                if i8t %15, goto lb55, else goto lb56;
                lb55:
                goto lb19;
                lb56:
                goto lb16;
                lb46:
                goto lb13;
                lb26:
                goto lb10;
                lb6:
                exit num? 1;
            }
        }
    }
}
:
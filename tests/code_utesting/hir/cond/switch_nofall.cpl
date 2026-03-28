start() {
    i32 a;
    @[no_fall] switch a as i64; {
        case 1; {}
        case 2; {}
        case 3; {}
        case 4; {}
        default {}
    }
    exit 1 as i8;
}

: OUTPUT
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
                lb26:
                {
                }
                goto lb13;
                lb30:
                {
                }
                goto lb13;
                goto lb13;
                lb12:
                i64t %1 = i32s %0 as i64;
                i8t %2 = i64t %1 < cnst? 2;
                if i8t %2, goto lb40, else goto lb41;
                lb40:
                i8t %3 = i64t %1 < cnst? 1;
                if i8t %3, goto lb46, else goto lb47;
                lb46:
                goto lb30;
                lb47:
                i8t %4 = i64t %1 > cnst? 1;
                if i8t %4, goto lb48, else goto lb49;
                lb48:
                goto lb30;
                lb49:
                goto lb14;
                lb41:
                i8t %5 = i64t %1 > cnst? 2;
                if i8t %5, goto lb42, else goto lb43;
                lb42:
                i8t %6 = i64t %1 < cnst? 3;
                if i8t %6, goto lb56, else goto lb57;
                lb56:
                goto lb30;
                lb57:
                i8t %7 = i64t %1 > cnst? 3;
                if i8t %7, goto lb58, else goto lb59;
                lb58:
                i8t %8 = i64t %1 < cnst? 4;
                if i8t %8, goto lb64, else goto lb65;
                lb64:
                goto lb30;
                lb65:
                i8t %9 = i64t %1 > cnst? 4;
                if i8t %9, goto lb66, else goto lb67;
                lb66:
                goto lb30;
                lb67:
                goto lb26;
                lb59:
                goto lb22;
                lb43:
                goto lb18;
                lb13:
                i8t %10 = num? 1 as i8;
                exit i8t %10;
            }
        }
    }
}
:
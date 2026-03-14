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
                goto lb{X}
                lb{X}
                {
                }
                goto lb{X}
                lb{X}
                {
                }
                goto lb{X}
                lb{X}
                {
                }
                goto lb{X}
                lb{X}
                {
                }
                goto lb{X}
                lb{X}
                {
                }
                goto lb{X}
                goto lb{X}
                lb{X}
                i32t %2 = cnst? 2 as i32;
                i8t %1 = i32s %0 < i32t %2;
                if i8t %1, goto lb{X} else goto lb{X}
                lb{X}
                i32t %4 = cnst? 1 as i32;
                i8t %3 = i32s %0 < i32t %4;
                if i8t %3, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                i32t %6 = cnst? 1 as i32;
                i8t %5 = i32s %0 > i32t %6;
                if i8t %5, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                i32t %8 = cnst? 2 as i32;
                i8t %7 = i32s %0 > i32t %8;
                if i8t %7, goto lb{X} else goto lb{X}
                lb{X}
                i32t %10 = cnst? 3 as i32;
                i8t %9 = i32s %0 < i32t %10;
                if i8t %9, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                i32t %12 = cnst? 3 as i32;
                i8t %11 = i32s %0 > i32t %12;
                if i8t %11, goto lb{X} else goto lb{X}
                lb{X}
                i32t %14 = cnst? 4 as i32;
                i8t %13 = i32s %0 < i32t %14;
                if i8t %13, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                i32t %16 = cnst? 4 as i32;
                i8t %15 = i32s %0 > i32t %16;
                if i8t %15, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                exit num? 1;
            }
        }
    }
}
:
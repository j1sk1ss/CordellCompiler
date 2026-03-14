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
                goto lb{X}
                lb{X}
                {
                }
                lb{X}
                {
                }
                lb{X}
                {
                }
                lb{X}
                {
                }
                lb{X}
                {
                }
                goto lb{X}
                lb{X}
                i8t %1 = i32s %0 == cnst? 1;
                if i8t %1, goto lb{X} else goto lb{X}
                lb{X}
                i8t %2 = i32s %0 == cnst? 2;
                if i8t %2, goto lb{X} else goto lb{X}
                lb{X}
                i8t %3 = i32s %0 == cnst? 3;
                if i8t %3, goto lb{X} else goto lb{X}
                lb{X}
                i8t %4 = i32s %0 == cnst? 4;
                if i8t %4, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
                exit num? 1;
            }
        }
    }
}
:
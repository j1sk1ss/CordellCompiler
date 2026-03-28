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
                i8t %1 = i32s %0 == cnst? 1;
                if i8t %1, goto lb{X} else goto lb{X}
                lb{X}
                i8t %2 = i32s %0 == cnst? 2;
                if i8t %2, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
            }
        }
    }
}
/:

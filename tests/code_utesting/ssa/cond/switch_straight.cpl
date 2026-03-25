start() {
    @[straight] switch 1; {
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
                goto lb{X}
                lb{X}
                i8t %0 = num? 1 == cnst? 1;
                if i8t %0, goto lb{X} else goto lb{X}
                lb{X}
                i8t %1 = num? 1 == cnst? 2;
                if i8t %1, goto lb{X} else goto lb{X}
                lb{X}
                goto lb{X}
                lb{X}
            }
        }
    }
}
:
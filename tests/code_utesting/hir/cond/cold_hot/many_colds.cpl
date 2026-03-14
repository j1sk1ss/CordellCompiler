start() {
    @[cold] if 1; {
        exit 1;
    }
    else {
        exit 2;
    }

    @[cold] if 2; {
        exit 1;
    }
    else {
        exit 2;
    }

    @[cold] if 3; {
        exit 1;
    }
    else {
        exit 2;
    }
}

: OUTPUT
{
    start {
        {
            {
                if num? 1, goto lb{X} else goto lb{X}
                lb{X}
                {
                    exit num? 2;
                }
                goto lb{X}
                lb{X}
                if num? 2, goto lb{X} else goto lb{X}
                lb{X}
                {
                    exit num? 2;
                }
                goto lb{X}
                lb{X}
                if num? 3, goto lb{X} else goto lb{X}
                lb{X}
                {
                    exit num? 2;
                }
                goto lb{X}
                lb{X}
            }
            lb{X}
            {
                exit num? 1;
            }
            goto lb{X}
            lb{X}
            {
                exit num? 1;
            }
            goto lb{X}
            lb{X}
            {
                exit num? 1;
            }
            goto lb{X}
        }
    }
}
:
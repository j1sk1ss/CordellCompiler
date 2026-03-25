@[entry]
function foo(i32 a) {
    if a == 1; {

    }
    else if a == 2; {

    }
    else if a == 3; {

    }
    else {

    }
}

: OUTPUT
{
    fn _main(i32 a)
    {
        i32s %0 = alloc(8);
        i32s %0 = load_starg();
        {
            i64t %2 = i32s %0 as i64;
            i64t %1 = i64t %2 == num? 1;
            if i64t %1, goto lb{X} else goto lb{X}
            lb{X}
            {
            }
            goto lb{X}
            lb{X}
            {
                i64t %4 = i32s %0 as i64;
                i64t %3 = i64t %4 == num? 2;
                if i64t %3, goto lb{X} else goto lb{X}
                lb{X}
                {
                }
                goto lb{X}
                lb{X}
                {
                    i64t %6 = i32s %0 as i64;
                    i64t %5 = i64t %6 == num? 3;
                    if i64t %5, goto lb{X} else goto lb{X}
                    lb{X}
                    {
                    }
                    goto lb{X}
                    lb{X}
                    {
                    }
                    goto lb{X}
                    lb{X}
                }
                goto lb{X}
                lb{X}
            }
            goto lb{X}
            lb{X}
        }
    }
}
:
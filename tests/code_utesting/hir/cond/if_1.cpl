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

:/ OUTPUT
{
    fn _main(i32 a)
    {
        i32s %0 = alloc;
        i32s %0 = load_starg();
        {
            i32t %2 = i8n 1 as i32;
            i32t %1 = i32s %0 == i32t %2;
            if i32t %1, goto lb11, else goto lb12;
            lb11:
            {
            }
            goto lb13;
            lb12:
            {
                i32t %4 = i8n 2 as i32;
                i32t %3 = i32s %0 == i32t %4;
                if i32t %3, goto lb22, else goto lb23;
                lb22:
                {
                }
                goto lb24;
                lb23:
                {
                    i32t %6 = i8n 3 as i32;
                    i32t %5 = i32s %0 == i32t %6;
                    if i32t %5, goto lb33, else goto lb34;
                    lb33:
                    {
                    }
                    goto lb35;
                    lb34:
                    {
                    }
                    goto lb35;
                    lb35:
                }
                goto lb24;
                lb24:
            }
            goto lb13;
            lb13:
        }
    }
}
/:
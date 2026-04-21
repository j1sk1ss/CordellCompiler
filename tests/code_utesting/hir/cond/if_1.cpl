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
        i32s %0 = alloc(8);
        i32s %0 = load_starg();
        {
            i32t %2 = i8n 1 as i32;
            i32t %1 = i32s %0 == i32t %2;
            if i32t %1, goto lb10, else goto lb11;
            lb10:
            {
            }
            goto lb12;
            lb11:
            {
                i32t %4 = i8n 2 as i32;
                i32t %3 = i32s %0 == i32t %4;
                if i32t %3, goto lb21, else goto lb22;
                lb21:
                {
                }
                goto lb23;
                lb22:
                {
                    i32t %6 = i8n 3 as i32;
                    i32t %5 = i32s %0 == i32t %6;
                    if i32t %5, goto lb32, else goto lb33;
                    lb32:
                    {
                    }
                    goto lb34;
                    lb33:
                    {
                    }
                    goto lb34;
                    lb34:
                }
                goto lb23;
                lb23:
            }
            goto lb12;
            lb12:
        }
    }
}
/:
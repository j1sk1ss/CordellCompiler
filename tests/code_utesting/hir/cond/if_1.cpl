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
            if i32t %1, goto lb13, else goto lb14;
            lb13:
            {
            }
            goto lb15;
            lb14:
            {
                i32t %4 = i8n 2 as i32;
                i32t %3 = i32s %0 == i32t %4;
                if i32t %3, goto lb28, else goto lb29;
                lb28:
                {
                }
                goto lb30;
                lb29:
                {
                    i32t %6 = i8n 3 as i32;
                    i32t %5 = i32s %0 == i32t %6;
                    if i32t %5, goto lb43, else goto lb44;
                    lb43:
                    {
                    }
                    goto lb45;
                    lb44:
                    {
                    }
                    goto lb45;
                    lb45:
                }
                goto lb30;
                lb30:
            }
            goto lb15;
            lb15:
        }
    }
}
/:
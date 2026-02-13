{
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
}

: OUTPUT
{
    fn foo0(i32 a)
    {
        i32s %0 = alloc(8);
        i32s %0 = load_arg();
        {
            i64t %2 = i32s %0 as i64;
            i64t %1 = i64t %2 == num? 1;
            if i64t %1, goto lb7, else goto lb8;
            lb7:
            {
            }
            goto lb9;
            lb8:
            {
                i64t %4 = i32s %0 as i64;
                i64t %3 = i64t %4 == num? 2;
                if i64t %3, goto lb17, else goto lb18;
                lb17:
                {
                }
                goto lb19;
                lb18:
                {
                    i64t %6 = i32s %0 as i64;
                    i64t %5 = i64t %6 == num? 3;
                    if i64t %5, goto lb27, else goto lb28;
                    lb27:
                    {
                    }
                    goto lb29;
                    lb28:
                    {
                    }
                    goto lb29;
                    lb29:
                }
                goto lb19;
                lb19:
            }
            goto lb9;
            lb9:
        }
    }
}
:
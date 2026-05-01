@[entry]
function foo(i32 a) {
    if a > 10; {
        return 1;
    }

    if a > 10; {
        return 10;
    }

    return 20;
}

:/ OUTPUT
{
    fn _main(i32 a)
    {
        i32s %0 = alloc;
        i32s %5 = load_starg();
        {
            i32t %2 = i8n 10 as i32;
            i32t %1 = i32s %5 > i32t %2;
            if i32t %1, goto lb10, else goto lb12;
            lb10:
            {
                return i8n 1;
            }
            goto lb12;
            lb12:
            i32t %4 = i8n 10 as i32;
            i32t %3 = i32s %5 > i32t %4;
            if i32t %3, goto lb22, else goto lb24;
            lb22:
            {
                return i8n 10;
            }
            goto lb24;
            lb24:
            return i8n 20;
        }
    }
    [unused]
}
/:
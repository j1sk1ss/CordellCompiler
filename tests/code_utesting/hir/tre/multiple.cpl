@[entry]
function foo(i32 a) {
    if a > 10; {
        return foo(a - 1);
    }
    return foo(a + 1);
}

:/ OUTPUT
{
    fn _main(i32 a)
    {
        i32s %0 = alloc(8);
        i32s %0 = load_starg();
        {
            lb40:
            i32t %2 = i8n 10 as i32;
            i32t %1 = i32s %0 > i32t %2;
            if i32t %1, goto lb10, else goto lb12;
            lb10:
            {
                i32t %4 = i8n 1 as i32;
                i32t %3 = i32s %0 - i32t %4;
                use i32t %3;
                i32s %0 = i32t %3;
                goto lb40;
                [unused] i64t %5 = call _main(i32 a), argc args(i32t %3,);
                return i64t %5;
            }
            goto lb12;
            lb12:
            i32t %7 = i8n 1 as i32;
            i32t %6 = i32s %0 + i32t %7;
            use i32t %6;
            i32s %0 = i32t %6;
            goto lb40;
            [unused] i64t %8 = call _main(i32 a), argc args(i32t %6,);
            return i64t %8;
        }
    }
    [unused]
}
/:
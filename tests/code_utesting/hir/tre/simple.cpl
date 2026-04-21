@[entry]
function foo(i32 a) {
    if a > 10; {
        return;
    }

    foo(a + 1);
}

:/ OUTPUT
{
    fn _main(i32 a)
    {
        i32s %0 = alloc(8);
        i32s %0 = load_starg();
        {
            lb31:
            i32t %2 = i8n 10 as i32;
            i32t %1 = i32s %0 > i32t %2;
            if i32t %1, goto lb10, else goto lb12;
            lb10:
            {
                return ;
            }
            goto lb12;
            lb12:
            i32t %4 = i8n 1 as i32;
            i32t %3 = i32s %0 + i32t %4;
            use i32t %3;
            i32s %0 = i32t %3;
            goto lb31;
            [unused] call _main(i32 a), argc args(i32t %3,);
        }
    }
}
/:
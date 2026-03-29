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
            lb41:
            i32t %2 = i8n 10 as i32;
            i32t %1 = i32s %0 > i32t %2;
            if i32t %1, goto lb13, else goto lb15;
            lb13:
            {
                return ;
            }
            goto lb15;
            lb15:
            i32t %4 = i8n 1 as i32;
            i32t %3 = i32s %0 + i32t %4;
            use i32t %3;
            i32s %0 = i32t %3;
            goto lb41;
            [unused] call _main(i32 a), argc args(i32t %3,);
        }
    }
}
/:
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
        i32s %0 = alloc;
        i32s %0 = load_starg();
        {
            lb32:
            i32t %2 = i8n 10 as i32;
            i32t %1 = i32s %0 > i32t %2;
            if i32t %1, goto lb11, else goto lb13;
            lb11:
            {
                return ;
            }
            goto lb13;
            lb13:
            i32t %4 = i8n 1 as i32;
            i32t %3 = i32s %0 + i32t %4;
            use i32t %3;
            i32s %0 = i32t %3;
            goto lb32;
            [unused] _main(i32t %3);
        }
    }
}
/:
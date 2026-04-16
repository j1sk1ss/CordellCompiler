@[entry]
function foo() {
    i32 a = 1;
    if 1; {
        a = 0;
    }
    ptr i32 b = a as ptr i32;
    exit dref b;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s %0 = alloc(8);
            i32t %2 = i8n 1 as i32;
            i32s %6 = i32t %2;
            future: i32s %7 = previous: i32s %6;
            if i8n 1, goto lb18, else goto lb20;
            lb18:
            {
                i32t %3 = i8n 0 as i32;
                i32s %9 = i32t %3;
            }
            future: i32s %7 = previous: i32s %9;
            goto lb20;
            lb20:
            [i32s %0] i32s %7 = phi(set [%6, bb1][%9, bb2]);
            i32s* %1 = alloc(8);
            i32t* %4 = i32s %7 as *ptr;
            i32s* %8 = i32t* %4;
            i32t %5 = *(i32s* %8);
            exit i32t %5;
        }
    }
    [unused]
}
/:
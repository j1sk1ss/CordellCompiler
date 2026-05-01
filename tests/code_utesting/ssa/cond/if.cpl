start() {
    i32 a = 0;
    if 1; {
        a = 1;
    }
    else {
        a = 2;
    }
    i32 b = a;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s %0 = alloc;
            i32t %2 = i8n 0 as i32;
            i32s %5 = i32t %2;
            if i8n 1, goto lb12, else goto lb13;
            lb12:
            {
                i32t %3 = i8n 1 as i32;
                i32s %9 = i32t %3;
            }
            [SSA] future: i32s %7 <<== previous: i32s %9;
            goto lb14;
            lb13:
            {
                i32t %4 = i8n 2 as i32;
                i32s %6 = i32t %4;
            }
            [SSA] future: i32s %7 <<== previous: i32s %6;
            goto lb14;
            lb14:
            [base: i32s %0] i32s %7 = phi(set [%9, bb2][%6, bb3]);
            i32s %1 = alloc;
            i32s %8 = i32s %7;
        }
    }
}
/:
start() {
    i32 a = 0;
    while 1; {
        a = 1;
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
            i32s %4 = i32t %2;
            [SSA] future: i32s %5 <<== previous: i32s %4;
            i32t %3 = i8n 1 as i32;
            i32s %7 = i32t %3;
            lb12:
            [base: i32s %0] i32s %5 = phi(set [%7, bb3][%4, bb5]);
            if i8n 1, goto lb13, else goto lb14;
            lb13:
            {
            }
            [SSA] future: i32s %5 <<== previous: i32s %7;
            goto lb12;
            lb14:
            i32s %1 = alloc;
            i32s %6 = i32s %5;
        }
    }
}
/:
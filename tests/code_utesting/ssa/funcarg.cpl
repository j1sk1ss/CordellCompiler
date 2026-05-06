start() {
    ptr i32 a = 1;
    if 1; {
        if 2; a = 0;
        else a = 2;
        a = 1;
    }
    dref a = 0;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s* %0 = alloc;
            i32t* %1 = i8n 1 as *ptr;
            i32s* %6 = i32t* %1;
            [SSA] future: i32s* %7 <<== previous: i32s* %6;
            if i8n 1, goto lb12, else goto lb14;
            lb12:
            {
                if i8n 2, goto lb18, else goto lb19;
                lb18:
                {
                    i32t* %2 = i8n 0 as *ptr;
                    i32s* %11 = i32t* %2;
                }
                [SSA] future: i32s* %9 <<== previous: i32s* %11;
                goto lb20;
                lb19:
                {
                    i32t* %3 = i8n 2 as *ptr;
                    i32s* %8 = i32t* %3;
                }
                [SSA] future: i32s* %9 <<== previous: i32s* %8;
                goto lb20;
                lb20:
                [base: i32s %0] i32s* %9 = phi(set [%8, bb4][%11, bb3]);
                i32t* %4 = i8n 1 as *ptr;
                i32s* %10 = i32t* %4;
            }
            [SSA] future: i32s* %7 <<== previous: i32s* %10;
            goto lb14;
            lb14:
            [base: i32s %0] i32s* %7 = phi(set [%10, bb5][%6, bb1]);
            i32t %5 = i8n 0 as i32;
            *(i32s* %7) = i32t %5;
        }
    }
}
/:
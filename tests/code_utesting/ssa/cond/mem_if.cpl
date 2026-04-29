start() {
    ptr i0 p = 0x100;
    i32 a = 1;
    ptr i32 b = ref a;
    dref b = 0;
    if dref b == 0; {
        p = 0;
    }
    dref p = 1;
}

:/ OUTPUT
{
    start {
        {
            {
                i0s* %0 = alloc;
                i0t* %3 = i16n 256 as *ptr;
                i0s* %12 = i0t* %3;
                i32s %1 = alloc;
                i32t %4 = i8n 1 as i32;
                i32s %13 = i32t %4;
                i32s* %2 = alloc;
                i32t* %5 = &(i32s %13);
                i32s* %14 = i32t* %5;
                i32t %6 = i8n 0 as i32;
                *(i32s* %14) = i32t %6;
                i32t %7 = *(i32s* %14);
                i32t %9 = i8n 0 as i32;
                i32t %8 = i32t %7 == i32t %9;
                [SSA] future: i0s* %15 <<== previous: i0s* %12;
                if i32t %8, goto lb26, else goto lb28;
                lb26:
                {
                    i0t* %10 = i8n 0 as *ptr;
                    i0s* %16 = i0t* %10;
                }
                [SSA] future: i0s* %15 <<== previous: i0s* %16;
                goto lb28;
                lb28:
                [base: i0s %0] i0s* %15 = phi(set [%16, bb2][%12, bb1]);
                i0t %11 = i8n 1;
                *(i0s* %15) = i0t %11;
            }
        }
    }
}
/:
@[entry]
function foo() {
    ptr i32 a = 1;
    ptr i32 b = 1;
    if 1; {
        @[no_fall]
        @[straight]
        switch 1; {
            case 1; { a = 0; }
            case 2; { a = 1; }
            default { a = 1; }
        }
        b = a;
    }
    else {
        a = 2;
    }
    ptr i32 c = b;
    dref c;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s* %0 = alloc(8);
            i32t* %3 = i8n 1 as *ptr;
            i32s* %14 = i32t* %3;
            i32s* %1 = alloc(8);
            i32t* %4 = i8n 1 as *ptr;
            i32s* %15 = i32t* %4;
            if i8n 1, goto lb26, else goto lb27;
            lb26:
            {
                goto lb34;
                lb36:
                {
                    i32t* %5 = i8n 0 as *ptr;
                    i32s* %24 = i32t* %5;
                }
                future: u64s* %21 = previous: u64s* %24;
                goto lb35;
                lb47:
                {
                    i32t* %6 = i8n 1 as *ptr;
                    i32s* %23 = i32t* %6;
                }
                future: u64s* %21 = previous: u64s* %23;
                goto lb35;
                lb58:
                {
                    i32t* %7 = i8n 1 as *ptr;
                    i32s* %20 = i32t* %7;
                }
                future: u64s* %21 = previous: u64s* %20;
                goto lb35;
                [unused] goto lb35;
                lb34:
                i64t %9 = i8n 1;
                i8t %8 = i64t %9 == cnst? 1;
                if i8t %8, goto lb36, else goto lb71;
                lb71:
                i64t %11 = i8n 1;
                i8t %10 = i64t %11 == cnst? 2;
                if i8t %10, goto lb47, else goto lb75;
                lb75:
                goto lb58;
                lb35:
                [u64s %0] u64s* %21 = phi(set [%23, bb4][%20, bb5][%24, bb3]);
                i32s* %22 = i32s* %21;
            }
            future: u64s* %17 = previous: u64s* %21;
            future: u64s* %18 = previous: u64s* %22;
            goto lb28;
            lb27:
            {
                i32t* %12 = i8n 2 as *ptr;
                i32s* %16 = i32t* %12;
            }
            future: u64s* %17 = previous: u64s* %16;
            future: u64s* %18 = previous: u64s* %15;
            goto lb28;
            lb28:
            [u64s %0] u64s* %17 = phi(set [%21, bb10][%16, bb11]);
            [u64s %1] u64s* %18 = phi(set [%22, bb10][%15, bb11]);
            i32s* %2 = alloc(8);
            i32s* %19 = i32s* %18;
            i32t %13 = *(i32s* %19);
            use i32t %13;
        }
    }
}
/:
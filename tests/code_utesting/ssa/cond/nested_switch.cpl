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
            i32s* %16 = i32t* %3;
            i32s* %1 = alloc(8);
            i32t* %4 = i8n 1 as *ptr;
            i32s* %17 = i32t* %4;
            if i8n 1, goto lb17, else goto lb18;
            lb17:
            {
                goto lb23;
                lb25:
                {
                    i32t* %5 = i8n 0 as *ptr;
                    i32s* %26 = i32t* %5;
                }
                future: u64s* %23 = previous: u64s* %26;
                goto lb24;
                lb32:
                {
                    i32t* %6 = i8n 1 as *ptr;
                    i32s* %25 = i32t* %6;
                }
                future: u64s* %23 = previous: u64s* %25;
                goto lb24;
                lb39:
                {
                    i32t* %7 = i8n 1 as *ptr;
                    i32s* %22 = i32t* %7;
                }
                future: u64s* %23 = previous: u64s* %22;
                goto lb24;
                [unused] goto lb24;
                lb23:
                i64t %9 = csu8 1 as i64;
                i64t %10 = i8n 1 as i64;
                i8t %8 = i64t %10 == i64t %9;
                if i8t %8, goto lb25, else goto lb48;
                lb48:
                i64t %12 = csu8 2 as i64;
                i64t %13 = i8n 1 as i64;
                i8t %11 = i64t %13 == i64t %12;
                if i8t %11, goto lb32, else goto lb53;
                lb53:
                goto lb39;
                lb24:
                [u64s %0] u64s* %23 = phi(set [%22, bb5][%26, bb3][%25, bb4]);
                i32s* %24 = i32s* %23;
            }
            future: u64s* %19 = previous: u64s* %23;
            future: u64s* %20 = previous: u64s* %24;
            goto lb19;
            lb18:
            {
                i32t* %14 = i8n 2 as *ptr;
                i32s* %18 = i32t* %14;
            }
            future: u64s* %19 = previous: u64s* %18;
            future: u64s* %20 = previous: u64s* %17;
            goto lb19;
            lb19:
            [u64s %0] u64s* %19 = phi(set [%18, bb10][%23, bb9]);
            [u64s %1] u64s* %20 = phi(set [%17, bb10][%24, bb9]);
            i32s* %2 = alloc(8);
            i32s* %21 = i32s* %20;
            i32t %15 = *(i32s* %21);
            use i32t %15;
        }
    }
}
/:
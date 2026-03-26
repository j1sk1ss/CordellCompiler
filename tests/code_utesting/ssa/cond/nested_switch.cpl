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

: OUTPUT
{
    fn _main()
    {
        {
            i32s* %0 = alloc(8);
            i32t* %3 = num? 1 as *ptr;
            i32s* %12 = i32t* %3;
            i32s* %1 = alloc(8);
            i32t* %4 = num? 1 as *ptr;
            i32s* %13 = i32t* %4;
            if num? 1, goto lb28, else goto lb29;
            lb{X}
            {
                goto lb{X}
                lb{X}
                {
                    i32t* %5 = num? 0 as *ptr;
                    i32s* %22 = i32t* %5;
                }
                future: u64s* %19 = previous: u64s* %22;
                goto lb38;
                lb51:
                {
                    i32t* %6 = num? 1 as *ptr;
                    i32s* %21 = i32t* %6;
                }
                future: u64s* %19 = previous: u64s* %21;
                goto lb38;
                lb63:
                {
                    i32t* %7 = num? 1 as *ptr;
                    i32s* %18 = i32t* %7;
                }
                future: u64s* %19 = previous: u64s* %18;
                goto lb38;
                goto lb38;
                lb37:
                i8t %8 = num? 1 == cnst? 1;
                if i8t %8, goto lb39, else goto lb78;
                lb78:
                i8t %9 = num? 1 == cnst? 2;
                if i8t %9, goto lb51, else goto lb81;
                lb81:
                goto lb63;
                lb38:
                [u64s %0] u64s* %19 = phi(set [%18, bb5][%21, bb4][%22, bb3]);
                i32s* %20 = i32s* %19;
            }
            future: u64s* %15 = previous: u64s* %19;
            future: u64s* %16 = previous: u64s* %20;
            goto lb30;
            lb29:
            {
                i32t* %10 = num? 2 as *ptr;
                i32s* %14 = i32t* %10;
            }
            future: u64s* %15 = previous: u64s* %14;
            future: u64s* %16 = previous: u64s* %13;
            goto lb30;
            lb30:
            [u64s %0] u64s* %15 = phi(set [%19, bb10][%14, bb11]);
            [u64s %1] u64s* %16 = phi(set [%13, bb11][%20, bb10]);
            i32s* %2 = alloc(8);
            i32s* %17 = i32s* %16;
            i32t %11 = *(i32s* %17);
            use i32t %11;
        }
    }
}
:
@[entry]
function foo(i32 arg) {
    i32 a = 1;
    if arg > 10 && arg < 20; a = 0;
    ptr i32 b = a as ptr i32;
    exit dref b;
}

:/ OUTPUT
{
    fn _main(i32 arg)
    {
        i32s %0 = alloc;
        i32s %13 = load_starg();
        {
            i32s %1 = alloc;
            i32t %3 = i8n 1 as i32;
            i32s %14 = i32t %3;
            i64s %16 = alloc;
            i32t %6 = i8n 10 as i32;
            i32t %5 = i32s %13 > i32t %6;
            if i32t %5, goto lb23, else goto lb24;
            lb23:
            i32t %8 = i8n 20 as i32;
            i32t %7 = i32s %13 < i32t %8;
            i64s %16 = alloc;
            i64t %9 = i32t %7 as i64;
            i64s %20 = i64t %9;
            [SSA] future: i64s %16 <<== previous: i64s %20;
            goto lb25;
            lb24:
            i64s %15 = csu8 0;
            [SSA] future: i64s %16 <<== previous: i64s %15;
            goto lb25;
            lb25:
            [base: i64s %4] i64s %16 = phi(set [%20, bb2][%15, bb3]);
            [SSA] future: i32s %17 <<== previous: i32s %14;
            if i64s %16, goto lb15, else goto lb17;
            lb15:
            {
                i32t %10 = i8n 0 as i32;
                i32s %19 = i32t %10;
            }
            [SSA] future: i32s %17 <<== previous: i32s %19;
            goto lb17;
            lb17:
            [base: i32s %1] i32s %17 = phi(set [%19, bb5][%14, bb4]);
            i32s* %2 = alloc;
            i32t* %11 = i32s %17 as *ptr;
            i32s* %18 = i32t* %11;
            i32t %12 = *(i32s* %18);
            exit i32t %12;
        }
    }
    [unused]
}
/:
@[entry]
function foo() {
    ptr i32 a = 1;
    ptr i32 b = 1;
    if 1; {
        a = 0;
    }

    return dref a + dref b;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32s* %0 = alloc(8);
            i32t* %2 = i8n 1 as *ptr;
            i32s* %9 = i32t* %2;
            i32s* %1 = alloc(8);
            i32t* %3 = i8n 1 as *ptr;
            i32s* %10 = i32t* %3;
            future: u64s* %11 = previous: u64s* %9;
            if i8n 1, goto lb26, else goto lb28;
            lb26:
            {
                i32t* %4 = i8n 0 as *ptr;
                i32s* %12 = i32t* %4;
            }
            future: u64s* %11 = previous: u64s* %12;
            goto lb28;
            lb28:
            [u64s %0] u64s* %11 = phi(set [%12, bb2][%9, bb1]);
            i32t %5 = *(i32s* %10);
            i32t* %7 = i32t %5 as *ptr;
            i32t* %6 = i32s* %11 + i32t* %7;
            i32t %8 = *(i32t* %6);
            return i32t %8;
        }
    }
    [unused]
}
/:
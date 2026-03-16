@[entry]
function foo() {
    ptr i32 a = 1;
    ptr i32 b = 1;
    if 1; {
        a = 0;
    }

    return dref a + dref b;
}

: OUTPUT
{
    fn _main()
    {
        {
            i32s* %0 = alloc(8);
            i32t* %2 = num? 1 as *ptr;
            i32s* %9 = i32t* %2;
            i32s* %1 = alloc(8);
            i32t* %3 = num? 1 as *ptr;
            i32s* %10 = i32t* %3;
            future{X} u64s* %11 = previous{X} u64s* %9;
            if num? 1, goto lb28, else goto lb30;
            lb{X}
            {
                i32t* %4 = num? 0 as *ptr;
                i32s* %12 = i32t* %4;
            }
            future{X} u64s* %11 = previous{X} u64s* %12;
            goto lb30;
            lb{X}
            [u64s %0] u64s* %11 = phi(set [%12, bb2][%9, bb1]);
            i32t %5 = *(i32s* %10);
            i32t %7 = i32s* %11 as i32;
            i32t %6 = i32t %7 + i32t %5;
            i32t %8 = *(i32t %6);
            return i32t %8;
        }
    }
}
:
@[entry]
function foo() {
    i32 a = 1;
    if 1; {
        a = 0;
    }
    ptr i32 b = a as ptr i32;
    exit dref b;
}

: OUTPUT
{
    fn _main()
    {
        {
            i32s %0 = alloc(8);
            i32t %2 = num? 1 as i32;
            i32s %6 = i32t %2;
            future{X} i32s %7 = previous{X} i32s %6;
            if num? 1, goto lb19, else goto lb21;
            lb{X}
            {
                i32t %3 = num? 0 as i32;
                i32s %9 = i32t %3;
            }
            future{X} i32s %7 = previous{X} i32s %9;
            goto lb21;
            lb{X}
            [i32s %0] i32s %7 = phi(set [%9, bb2][%6, bb1]);
            i32s* %1 = alloc(8);
            i32t* %4 = i32s %7 as *ptr;
            i32s* %8 = i32t* %4;
            i32t %5 = *(i32s* %8);
            exit i32t %5;
        }
    }
}
:
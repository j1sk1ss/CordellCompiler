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
    fn foo0()
    {
        {
            i32s* %0 = alloc(8);
            i32t* %2 = num? 1 as *ptr;
            i32s* %0 = i32t* %2;
            i32s* %1 = alloc(8);
            i32t* %3 = num? 1 as *ptr;
            i32s* %1 = i32t* %3;
            if num? 1, goto lb28, else goto lb30;
            lb{X}
            {
                i32t* %4 = num? 0 as *ptr;
                i32s* %0 = i32t* %4;
            }
            goto lb30;
            lb{X}
            i32t %5 = *(i32s* %1);
            i32t %7 = i32s* %0 as i32;
            i32t %6 = i32t %7 + i32t %5;
            i32t %8 = *(i32t %6);
            return i32t %8;
        }
    }
}
:
{
    function foo(i32 a) -> i32 {
        arr array[10, i32] = { 0 };
        if a == 1; {
            return 0;
        }
        else {
            return 1;
        }
    }

    start() {
        i32 a = foo(10);
    }
}

: OUTPUT
{
    fn foo0(i32 a) -> i32
    {
        i32s %0 = alloc(8);
        i32s %0 = load_arg();
        {
            arrs %1 = arr_alloc(num? 10);
            i64t %4 = i32s %0 as i64;
            i64t %3 = i64t %4 == num? 1;
            if i64t %3, goto lb{X}, else goto lb{X};
            lb{X}:
            {
                return num? 0;
            }
            goto lb{X};
            lb{X}:
            {
                return num? 1;
            }
            goto lb{X};
            lb{X}:
        [unused] }
    [unused] }
    [unused] 
    start {
        {
            {
                i32s %2 = alloc(8);
                use num? 10;
                i32s %0 = num? 10;
                arrs %1 = arr_alloc(num? 10);
                i64t %4 = i32s %0 as i64;
                i64t %3 = i64t %4 == num? 1;
                if i64t %3, goto lb{X}, else goto lb{X};
                lb{X}:
                {
                    i32t %5 = num? 0;
                }
                goto lb{X};
                lb{X}:
                {
                    i32t %5 = num? 1;
                }
                goto lb{X};
                lb{X}:
                [unused] i32t %5 = call foo0(i32 a) -> i32, argc args(num? 10,);
                i32s %2 = i32t %5;
            }
        }
    }
}
:
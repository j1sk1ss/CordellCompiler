{
    function foo(i32 a) => i32 {
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
            use num? 0;
            arrs* %1 = arr_alloc(num? 10);
            i64t %4 = i32s %0 as i64;
            i64t %3 = i64t %4 == num? 1;
            if i64t %3, goto lb12, else goto lb13;
            lb12:
            {
                return num? 0;
            }
            goto lb14;
            lb13:
            {
                return num? 1;
            }
            goto lb14;
            lb14:
        }
    }
    start {
        {
            {
                i32s %2 = alloc(8);
                use num? 10;
                i32s %0 = num? 10;
                use num? 0;
                arrs* %1 = arr_alloc(num? 10);
                i64t %4 = i32s %0 as i64;
                i64t %3 = i64t %4 == num? 1;
                if i64t %3, goto lb62, else goto lb63;
                lb62:
                {
                    i32t %5 = num? 0;
                }
                goto lb64;
                lb63:
                {
                    i32t %5 = num? 1;
                }
                goto lb64;
                lb64:
                [unused] i32t %5 = call foo0(i32 a) -> i32, argc args(num? 10,);
                i32s %2 = i32t %5;
            }
        }
    }
}
:
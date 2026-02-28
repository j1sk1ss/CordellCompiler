{
    function min(i32 a, i32 b) -> i32;
    function max(i32 a, i32 b) -> i32;
    function logic(i32 a, i32 b, ptr u64 perform) -> i32 {
        return perform(a, b);
    }

    start() {
        logic(10, 20, min);
        logic(10, 20, max);
    }
}

: OUTPUT
{
    fn logic2(i32 a, i32 b, u64* perform) -> i32
    {
        i32s %4 = alloc(8);
        i32s %4 = load_arg();
        i32s %5 = alloc(8);
        i32s %5 = load_arg();
        u64s* %6 = alloc(8);
        u64s* %6 = load_arg();
        {
            use i32s %4;
            use i32s %5;
            i64t %7 = call u64s* %6, argc args(i32s %4,i32s %5,);
            return i64t %7;
        }
    }
    start {
        {
            {
                use num? 10;
                use num? 20;
                i0t* %8 = &(min0(i32 a, i32 b) -> i32);
                use i0t* %8;
                call logic2(i32 a, i32 b, u64* perform) -> i32, argc args(num? 10,num? 20,i0t* %8,);
                use num? 10;
                use num? 20;
                i0t* %9 = &(max1(i32 a, i32 b) -> i32);
                use i0t* %9;
                call logic2(i32 a, i32 b, u64* perform) -> i32, argc args(num? 10,num? 20,i0t* %9,);
            }
        }
    }
}
:
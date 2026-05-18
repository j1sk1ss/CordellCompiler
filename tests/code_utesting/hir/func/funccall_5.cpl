function min(i32 a, i32 b) -> i32;
function max(i32 a, i32 b) -> i32;
function logic(i32 a, i32 b, ptr u64 perform) -> i32 {
    return perform(a, b) as i32;
}

start() {
    logic(10, 20, min);
    logic(10, 20, max);
}

:/ OUTPUT
{
    fn logic2(i32 a, i32 b, u64* perform) -> i32
    {
        i32s %4 = alloc;
        i32s %4 = load_arg();
        i32s %5 = alloc;
        i32s %5 = load_arg();
        u64s* %6 = alloc;
        u64s* %6 = load_arg();
        {
            use i32s %4;
            use i32s %5;
            i64t %8 = u64s* %6(i32s %4, i32s %5);
            i32t %7 = i64t %8 as i32;
            return i32t %7;
        }
    }
    fn _main()
    {
        {
            i0t* %9 = &(min0(i32 a, i32 b) -> i32);
            use i0t* %9;
            logic2(i8n 10, i8n 20, i0t* %9);
            i0t* %10 = &(max1(i32 a, i32 b) -> i32);
            use i0t* %10;
            logic2(i8n 10, i8n 20, i0t* %10);
        }
    }
}
/:
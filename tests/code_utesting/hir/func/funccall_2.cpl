{
    function foo(i32 a) -> i32 {
        return a;
    }

    start() {
        ptr i0 a = foo;
        i32 b = a(10);
    }
}

: OUTPUT
{
    fn foo0(i32 a) -> i32
    {
        i32s %0 = alloc(8);
        i32s %0 = load_arg();
        {
            return i32s %0;
        }
    }
    start {
        {
            {
                i0s* %1 = alloc(8);
                i0t* %3 = &(foo0(i32 a) -> i32);
                i0s* %1 = i0t* %3;
                i32s %2 = alloc(8);
                use num? 10;
                i64t %4 = call i0s* %1, argc args(num? 10,);
                i32t %5 = i64t %4 as i32;
                i32s %2 = i32t %5;
            }
        }
    }
}
:
{
    function foo(i32 a) => i32;
    start() {
        ptr u32 a = foo;
        i32 b = a(10);
    }
}

: OUTPUT
{
    start {
        {
            {
                u32s* %1 = alloc(8);
                u64t* %3 = &(foo0(i32 a) -> i32);
                u32t* %4 = u64t* %3 as *ptr;
                u32s* %1 = u32t* %4;
                i32s %2 = alloc(8);
                use num? 10;
                i64t %5 = call u32s* %1, argc args(num? 10,);
                i32t %6 = i64t %5 as i32;
                i32s %2 = i32t %6;
            }
        }
    }
}
:
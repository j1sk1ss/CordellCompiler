function foo(i32 a) -> i32 {
    return a;
}

start() {
    ptr i0 a = foo;
    i32 b = a(10) as i32;
}

:/ OUTPUT
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
                i64t %5 = call i0s* %1, argc args(num? 10,);
                i32t %4 = i64t %5 as i32;
                i32s %2 = i32t %4;
            }
        }
    }
}
/:

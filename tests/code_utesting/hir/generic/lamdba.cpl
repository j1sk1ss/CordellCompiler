function foo<T>(T a) -> T {
    return ((i32 b) => b * b)(a) as T;
}
start() {
    foo<i32>(1);
    foo<i8>(1);
}

:/ OUTPUT
{
    fn _main()
    {
        {
            foo4__i32(i8n 1);
            foo3__i8(i8n 1);
        }
    }
    fn foo3__i8(i8 a) -> i8
    {
        i8s %3 = alloc;
        i8s %3 = load_arg();
        {
            goto lb26;
            fn __anon_function_lambda5(i32 b)
            {
                i32s %2 = alloc;
                i32s %2 = load_arg();
                {
                    i32t %7 = i32s %2 * i32s %2;
                    use i32t %7;
                }
            }
            lb26:
            i0t* %8 = &(__anon_function_lambda5(i32 b));
            use i8s %3;
            i64t %9 = i0t* %8(i8s %3);
            i8t %6 = i64t %9 as i8;
            return i8t %6;
        }
    }
    fn foo4__i32(i32 a) -> i32
    {
        i32s %5 = alloc;
        i32s %5 = load_arg();
        {
            goto lb55;
            fn __anon_function_lambda6(i32 b)
            {
                i32s %4 = alloc;
                i32s %4 = load_arg();
                {
                    i32t %11 = i32s %4 * i32s %4;
                    use i32t %11;
                }
            }
            lb55:
            i0t* %12 = &(__anon_function_lambda6(i32 b));
            use i32s %5;
            i64t %13 = i0t* %12(i32s %5);
            i32t %10 = i64t %13 as i32;
            return i32t %10;
        }
    }
}
/:
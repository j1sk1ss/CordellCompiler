function sum<U, T>() -> U {
    function _local(U a) {
        return a * 10;
    }
    return ((T a) => a * a)(_local(1)) as U;
}

start() {
    sum<i8, i64>();
}

:/ OUTPUT
{
    fn _main()
    {
        {
            sum4__i8__i64();
        }
    }
    fn sum4__i8__i64() -> i8
    {
        {
            goto lb17;
            fn _local6(i8 a)
            {
                i8s %3 = alloc;
                i8s %3 = load_arg();
                {
                    i8t %4 = i8s %3 * i8n 10;
                    return i8t %4;
                }
            }
            lb17:
            goto lb33;
            fn __anon_function_lambda5(i64 a)
            {
                i64s %2 = alloc;
                i64s %2 = load_arg();
                {
                    i64t %6 = i64s %2 * i64s %2;
                    use i64t %6;
                }
            }
            lb33:
            i0t* %7 = &(__anon_function_lambda5(i64 a));
            i64t %8 = _local6(i8n 1);
            use i64t %8;
            i64t %9 = i0t* %7(i64t %8);
            i8t %5 = i64t %9 as i8;
            return i8t %5;
        }
    }
}
/:
start() {
    exit ((i32 a) => { return a * a; })(10) as u8;
}

:/ OUTPUT
{
    fn _main()
    {
        {
            goto lb8;
            fn __anon_function_lambda1(i32 a)
            {
                i32s %0 = alloc;
                i32s %0 = load_arg();
                {
                    i32t %2 = i32s %0 * i32s %0;
                    return i32t %2;
                }
            }
            lb8:
            i0t* %3 = &(__anon_function_lambda1(i32 a));
            i64t %4 = i0t* %3(i8n 10);
            u8t %1 = i64t %4 as u8;
            exit u8t %1;
        }
    }
}
/:
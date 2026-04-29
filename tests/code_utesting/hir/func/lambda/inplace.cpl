start() {
    exit ((i32 a) => { return a * a; })(10);
}

:/ OUTPUT
{
    start {
        {
            {
                goto lb7;
                fn __anon_function_lambda1(i32 a)
                {
                    i32s %0 = alloc;
                    i32s %0 = load_arg();
                    {
                        i32t %1 = i32s %0 * i32s %0;
                        return i32t %1;
                    }
                }
                lb7:
                i0t* %2 = &(__anon_function_lambda1(i32 a));
                i64t %3 = i0t* %2(i8n 10);
                exit i64t %3;
            }
        }
    }
}
/:
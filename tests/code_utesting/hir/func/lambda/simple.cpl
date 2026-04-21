start() {
    ptr i0 f = (i32 a) => { return a * 10; }; 
    exit f(1);
}

:/ OUTPUT
{
    start {
        {
            {
                i0s* %0 = alloc(8);
                goto lb8;
                fn __anon_function_lambda1(i32 a)
                {
                    i32s %1 = alloc(8);
                    i32s %1 = load_arg();
                    {
                        i32t %3 = i8n 10 as i32;
                        i32t %2 = i32s %1 * i32t %3;
                        return i32t %2;
                    }
                }
                lb8:
                i0t* %4 = &(__anon_function_lambda1(i32 a));
                i0s* %0 = i0t* %4;
                i64t %5 = call i0s* %0, argc args(i8n 1,);
                exit i64t %5;
            }
        }
    }
}
/:
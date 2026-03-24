start() {
    ptr i0 f = (i32 a) => { return a * 10; }; 
    exit f(1);
}

: OUTPUT
{
    start {
        {
            {
                i0s* %0 = alloc(8);
                goto lb{X};
                fn __anon_function_lambda0(i32 a)
                {
                    i32s %1 = alloc(8);
                    i32s %1 = load_arg();
                    {
                        i64t %3 = i32s %1 as i64;
                        i64t %2 = i64t %3 * num? 10;
                        return i64t %2;
                    }
                }
                lb{X}
                i0t* %4 = &(__anon_function_lambda0(i32 a));
                i0s* %0 = i0t* %4;
                i64t %5 = call i0s* %0, argc args(num? 1,);
                exit i64t %5;
            }
        }
    }
}
:
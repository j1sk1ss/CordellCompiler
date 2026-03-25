start() {
    exit ((i32 a) => { return a * a; })(10);
}

: OUTPUT
{
    start {
        {
            {
                goto lb{X};
                fn __anon_function_lambda0(i32 a)
                {
                    i32s %0 = alloc(8);
                    i32s %0 = load_arg();
                    {
                        i32t %1 = i32s %0 * i32s %0;
                        return i32t %1;
                    }
                }
                lb{X}
                i0t* %2 = &(__anon_function_lambda0(i32 a));
                i64t %3 = call i0t* %2, argc args(num? 10,);
                exit i64t %3;
            }
        }
    }
}
:
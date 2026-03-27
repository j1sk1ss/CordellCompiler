start() {
    i32 a = ((i32 a) => a)(10);
    function local(i32 a = 10) {
        return a;
    }
    i32 b = local();
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                goto lb{X}
                fn __anon_function_lambda1(i32 a)
                {
                    i32s %1 = alloc(8);
                    i32s %1 = load_arg();
                    {
                        return i32s %1;
                    }
                }
                lb{X}
                i0t* %4 = &(__anon_function_lambda1(i32 a));
                i64t %5 = call i0t* %4, argc args(num? 10,);
                i32t %6 = i64t %5 as i32;
                i32s %0 = i32t %6;
                goto lb{X}
                fn local2(i32 a)
                {
                    i32s %2 = alloc(8);
                    i32s %2 = load_arg();
                    {
                        return i32s %2;
                    }
                }
                lb{X}
                i32s %3 = alloc(8);
                i64t %7 = call local2(i32 a), argc args(num? 10,);
                i32t %8 = i64t %7 as i32;
                i32s %3 = i32t %8;
            }
        }
    }
}
:
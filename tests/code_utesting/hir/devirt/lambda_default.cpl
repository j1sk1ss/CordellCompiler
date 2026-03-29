start() {
    i32 a = ((i32 a) => a)(10) as i32;
    function local(i32 a = 10) {
        return a;
    }
    i32 b = local() as i32;
}

:/ OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                goto lb16;
                fn __anon_function_lambda1(i32 a)
                {
                    i32s %1 = alloc(8);
                    i32s %1 = load_arg();
                    {
                        return i32s %1;
                    }
                }
                lb16:
                i0t* %5 = &(__anon_function_lambda1(i32 a));
                i64t %6 = call i0t* %5, argc args(i8n 10,);
                i32t %4 = i64t %6 as i32;
                i32s %0 = i32t %4;
                goto lb38;
                fn local2(i32 a)
                {
                    i32s %2 = alloc(8);
                    i32s %2 = load_arg();
                    {
                        return i32s %2;
                    }
                }
                lb38:
                i32s %3 = alloc(8);
                i64t %8 = call local2(i32 a), argc args(i8n 10,);
                i32t %7 = i64t %8 as i32;
                i32s %3 = i32t %7;
            }
        }
    }
}
/:
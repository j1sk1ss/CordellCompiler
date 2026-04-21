function logic(i32 a, i32 b, ptr i0 f) {
    return f(a, b);
}
start() {
    logic(10, 20, (i32 a, i32 b) => a + b);
}

:/ OUTPUT
{
    fn logic0(i32 a, i32 b, i0* f)
    {
        i32s %0 = alloc(8);
        i32s %0 = load_arg();
        i32s %1 = alloc(8);
        i32s %1 = load_arg();
        i0s* %2 = alloc(8);
        i0s* %2 = load_arg();
        {
            use i32s %0;
            use i32s %1;
            i64t %5 = call i0s* %2, argc args(i32s %0,i32s %1,);
            return i64t %5;
        }
    }
    [unused]
    start {
        {
            {
                goto lb34;
                fn __anon_function_lambda2(i32 a, i32 b)
                {
                    i32s %3 = alloc(8);
                    i32s %3 = load_arg();
                    i32s %4 = alloc(8);
                    i32s %4 = load_arg();
                    {
                        i32t %6 = i32s %3 + i32s %4;
                        return i32t %6;
                    }
                }
                lb34:
                i0t* %7 = &(__anon_function_lambda2(i32 a, i32 b));
                use i0t* %7;
                call logic0(i32 a, i32 b, i0* f), argc args(i8n 10,i8n 20,i0t* %7,);
            }
        }
    }
}
/:
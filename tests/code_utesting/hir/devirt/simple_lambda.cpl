function logic(i32 a, i32 b, ptr i0 f) {
    return f(a, b);
}
start() {
    i32 a = (() => { 10; })() as i32;
    logic(10, 20, (i32 a, i32 b) => { a + b; });
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
            i64t %6 = call i0s* %2, argc args(i32s %0,i32s %1,);
            return i64t %6;
        }
    }
    [unused]
    start {
        {
            {
                i32s %3 = alloc(8);
                goto lb32;
                fn __anon_function_lambda2()
                {
                    {
                        return i8n 10;
                    }
                }
                lb32:
                i0t* %8 = &(__anon_function_lambda2());
                i64t %9 = call i0t* %8, argc args();
                i32t %7 = i64t %9 as i32;
                i32s %3 = i32t %7;
                goto lb49;
                fn __anon_function_lambda3(i32 a, i32 b)
                {
                    i32s %3 = alloc(8);
                    i32s %3 = load_arg();
                    i32s %5 = alloc(8);
                    i32s %5 = load_arg();
                    {
                        i32t %10 = i32s %3 + i32s %5;
                        return i32t %10;
                    }
                }
                lb49:
                i0t* %11 = &(__anon_function_lambda3(i32 a, i32 b));
                use i0t* %11;
                call logic0(i32 a, i32 b, i0* f), argc args(i8n 10,i8n 20,i0t* %11,);
            }
        }
    }
}
/:
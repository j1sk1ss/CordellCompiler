function foo(ptr i0 f);
start() {
    foo((i32 a) => a * a);
}

: OUTPUT
setpos, line=1, column=10, file=<unknown>
{
setpos, line=1, column=10, file=<unknown>
setpos, line=2, column=7, file=<unknown>
    start {
        {
setpos, line=3, column=9, file=<unknown>
            {
setpos, line=3, column=9, file=<unknown>
setpos, line=3, column=9, file=<unknown>
setpos, line=3, column=17, file=<unknown>
setpos, line=3, column=17, file=<unknown>
                goto lb{X}
                fn __anon_function_lambda2(i32 a)
                {
                    i32s %1 = alloc(8);
                    i32s %1 = load_arg();
setpos, line=3, column=29, file=<unknown>
                    {
setpos, line=3, column=32, file=<unknown>
setpos, line=3, column=32, file=<unknown>
setpos, line=3, column=29, file=<unknown>
setpos, line=3, column=29, file=<unknown>
setpos, line=3, column=35, file=<unknown>
setpos, line=3, column=35, file=<unknown>
                        i32t %2 = i32s %1 * i32s %1;
                        use i32t %2;
                    }
                }
                lb{X}
                i0t* %3 = &(__anon_function_lambda2(i32 a));
                use i0t* %3;
                call foo0(i0* f), argc args(i0t* %3,);
            }
        }
    }
}
:
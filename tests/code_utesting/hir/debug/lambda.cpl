function foo(ptr i0 f);
start() {
    foo((i32 a) => a * a);
}

:/ OUTPUT
setpos, line=1, column=10, file={X}
{
setpos, line=1, column=10, file={X}
setpos, line=2, column=7, file={X}
    start {
        {
setpos, line=3, column=9, file={X}
            {
setpos, line=3, column=9, file={X}
setpos, line=3, column=9, file={X}
setpos, line=3, column=17, file={X}
                goto lb13;
                fn __anon_function_lambda2(i32 a)
                {
                    i32s %1 = alloc(8);
                    i32s %1 = load_arg();
setpos, line=3, column=29, file={X}
                    {
setpos, line=3, column=32, file={X}
setpos, line=3, column=32, file={X}
setpos, line=3, column=29, file={X}
setpos, line=3, column=35, file={X}
                        i32t %2 = i32s %1 * i32s %1;
                        use i32t %2;
                    }
                }
                lb13:
                i0t* %3 = &(__anon_function_lambda2(i32 a));
                use i0t* %3;
                call foo0(i0* f), argc args(i0t* %3,);
            }
        }
    }
}
/:

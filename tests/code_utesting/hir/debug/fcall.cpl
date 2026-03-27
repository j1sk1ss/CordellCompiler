function foo() -> i32;
start() {
    exit foo();
}

: OUTPUT
setpos, line=1, column=10, file=<unknown>
{
setpos, line=1, column=10, file=<unknown>
setpos, line=2, column=7, file=<unknown>
    start {
        {
setpos, line=3, column=10, file=<unknown>
            {
setpos, line=3, column=10, file=<unknown>
setpos, line=3, column=10, file=<unknown>
setpos, line=3, column=15, file=<unknown>
setpos, line=3, column=15, file=<unknown>
                i32t %0 = call foo0() -> i32, argc args();
                exit i32t %0;
            }
        }
    }
}
:
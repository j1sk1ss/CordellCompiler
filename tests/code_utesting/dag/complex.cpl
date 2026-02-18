{
    function foo() -> i32;
    start() {
        i32 a = foo();
        i32 b = foo(); : The second function doesn't changed given the possibility of a side effect :
        i32 c = (a + b) + (a + b) + (a + b);
    }
}

: OUTPUT
{
    start {
        {
            {
                i32s %0 = alloc(8);
                i32t %3 = call foo0() -> i32, argc args();
                i32s %10 = i32t %3;
                i32s %1 = alloc(8);
                i32t %4 = call foo0() -> i32, argc args();
                i32s %11 = i32t %4;
                i32s %2 = alloc(8);
                i32t %5 = i32s %10 + i32s %11;
                [unused] i32t %6 = i32s %10 + i32s %11;
                i32t %7 = i32t %5 + i32t %5;
                [unused] i32t %8 = i32s %10 + i32s %11;
                i32t %9 = i32t %7 + i32t %5;
                i32s %12 = i32t %9;
            }
        }
    }
}
:
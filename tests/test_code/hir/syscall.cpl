{
    function foo() => i32;
    start() {
        syscall(0, foo(), 0 + 0, 0);
    }
}

: OUTPUT
{
    start {
        {
            {
                use num? 0;
                i32t %0 = call foo0() -> i32, argc args();
                use i32t %0;
                i64t %1 = num? 0 + num? 0;
                use i64t %1;
                use num? 0;
                syscall, argc: args(num? 0,i32t %0,i64t %1,num? 0,);
            }
        }
    }
}
:
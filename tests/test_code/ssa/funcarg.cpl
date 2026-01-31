{
    function a() => i32;
    function b(i32 g) => i32;
    function c(i32 f = b(a()));
    start() {
        c();
    }
}

: OUTPUT
{
    start {
        {
            {
                i32t %2 = call a0() -> i32, argc args();
                use i32t %2;
                i32t %3 = call b1(i32 g) -> i32, argc args(i32t %2,);
                use i32t %3;
                call c2(i32 f), argc args(i32t %3,);
            }
        }
    }
}
:
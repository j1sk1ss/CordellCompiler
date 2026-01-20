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
                i32t %2 = call a() -> i32, argc args();
                use i32t %2;
                i32t %3 = call b(i32 g) -> i32, argc args(i32t %2,);
                use i32t %3;
                call c(i32 f), argc args(i32t %3,);
            }
        }
    }
}
:
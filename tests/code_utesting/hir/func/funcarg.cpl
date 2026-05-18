function a() -> i32;
function b(i32 g) -> i32;
function c(i32 f = b(a()));
start() {
    c();
}

:/ OUTPUT
{
    fn _main()
    {
        {
            i32t %2 = a0();
            use i32t %2;
            i32t %3 = b1(i32t %2);
            use i32t %3;
            c2(i32t %3);
        }
    }
}
/:
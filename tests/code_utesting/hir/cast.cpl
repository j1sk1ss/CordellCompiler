{
    function foo() {
        i32 a;
        ptr ptr i32 b = a as ptr ptr i32;
    }
}

: OUTPUT
{
    fn _main0()
    {
        {
            i32s %0 = alloc(8);
            i32s** %1 = alloc(8);
            i32t** %2 = i32s %0 as *ptr;
            i32s** %1 = i32t** %2;
        }
    }
}
:
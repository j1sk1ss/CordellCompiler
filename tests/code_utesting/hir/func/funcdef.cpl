{
    function bar(i32 a);
    function bar(i32 a) {}
    function foo() -> i32 {
        return 0;
    }
}

: OUTPUT
{
    fn bar0(i32 a)
    {
        i32s %1 = alloc(8);
        i32s %1 = load_arg();
        {
        }
    }

    fn foo1() -> i32
    {
        {
            return num? 0;
        }
    }
}
:
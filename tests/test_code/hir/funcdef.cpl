{
    function bar(i32 a);
    function foo() => i32 {
        return 0;
    }

    function bar(i32 a) {}
}

: OUTPUT
{
    fn foo() -> i32
    {
        {
            return num? 0;
        }
    }
    
    fn bar(i32 a)
    {
        i32s %1 = alloc(8);
        i32s %1 = load_arg();
        {
        }
    }
}
:
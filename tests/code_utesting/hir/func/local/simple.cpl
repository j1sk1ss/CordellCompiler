@[entry]
function foo() {
    function bar() {
    }
}

:/ OUTPUT
{
    fn _main()
    {
        {
            goto lb7;
            fn bar1()
            {
                {
                }
            }
            lb7:
        }
    }
}
/:
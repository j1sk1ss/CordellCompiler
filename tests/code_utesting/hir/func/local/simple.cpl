@[entry]
function foo() {
    function bar() {
    }
}

: OUTPUT
{
    fn _main()
    {
        {
            goto lb4;
            fn bar1()
            {
                {
                }
            }
            lb4:
        }
    }
}
:
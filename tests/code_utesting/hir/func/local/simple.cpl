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
            goto lb{X}
            fn bar1()
            {
                {
                }
            }
            lb{X}
        }
    }
}
/:

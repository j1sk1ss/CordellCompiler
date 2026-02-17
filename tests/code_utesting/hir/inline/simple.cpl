{
    function getter() -> i32 {
        return 100;
    }

    start() {
        exit getter();
    }
}

: OUTPUT
{
    fn getter0() -> i32
    {
        {
            return num? 100;
        }
    }
    start {
        {
            {
                i32t %0 = num? 100;
                [unused] i32t %0 = call getter0() -> i32, argc args();
                exit i32t %0;
            }
        }
    }
}
:
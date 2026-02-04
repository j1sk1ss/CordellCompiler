{
    function getter() => i32 {
        return 100;
    }

    start() {
        getter();
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
                [unused] call getter0() -> i32, argc args();
            }
        }
    }
}
:
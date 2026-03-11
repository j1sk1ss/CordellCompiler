{
    @[entry]
    function foo() -> i32 {
        @[cold] if 1; {
            return 2;
        }
        else {
        }
        return 0;
    }
}

: OUTPUT
{
    fn _main() -> i32
    {
        {
            if num? 1, goto lb4, else goto lb5;
            lb5:
            {
            }
            goto lb6;
            lb6:
            return num? 0;
        }
        lb4:
        {
            return num? 2;
        }
        goto lb6;
    }
}
:
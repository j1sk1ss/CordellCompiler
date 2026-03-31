@[entry]
function foo() -> i32 {
    @[cold] if 1; {
        return 2;
    }
    else {
    }
    return 0;
}

:/ OUTPUT
{
    fn _main() -> i32
    {
        {
            if i8n 1, goto lb10, else goto lb11;
            lb11:
            {
            }
            goto lb12;
            lb12:
            return i8n 0;
        }
        exit cnst? 0;
        lb10:
        {
            return i8n 2;
        }
        goto lb12;
    }
}
/:
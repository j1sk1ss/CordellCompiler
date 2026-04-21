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
            if i8n 1, goto lb7, else goto lb8;
            lb8:
            {
            }
            goto lb9;
            lb9:
            return i8n 0;
        }
        exit csu8 0;
        lb7:
        {
            return i8n 2;
        }
        goto lb9;
    }
}
/:
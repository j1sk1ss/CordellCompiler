@[entry]
function foo() -> i32 {
    @[cold] if 1; {
        return 2;
    }
    else {
    }
    return 0;
}

: OUTPUT
{
    fn _main() -> i32
    {
        {
            if num? 1, goto lb{X} else goto lb{X}
            lb{X}
            {
            }
            goto lb{X}
            lb{X}
            return num? 0;
        }
        exit cnst? 0;
        lb{X}
        {
            return num? 2;
        }
        goto lb{X}
    }
}
:
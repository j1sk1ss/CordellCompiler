@[entry]
function foo() {
    ptr i32 a = 1;
    ptr i32 b = 1;

    @[straight]
    @[no_fall]
    switch 1; {
        case 1; { a = 3; }
        case 2; { a = 2; }
        case 3; { a = 0; }
        default { a = 0; }
    }

    @[straight]
    @[no_fall]
    switch 1; {
        case 1; { b = 1; }
        case 2; { b = 2; }
        default { b = 3; }
    }

    dref a = 1;
    dref b = 1;
}

: OUTPUT
#
{[WARNING] [8{X}] 'If' with a constant value (variable 'tmp' is equals 'true')!}
{[WARNING] [8{X}] 'If' with a constant value (variable 'tmp' is equals 'false')!}
{[WARNING] [8{X}] 'If' with a constant value (variable 'tmp' is equals 'false')!}
{[WARNING] [17{X}] 'If' with a constant value (variable 'tmp' is equals 'true')!}
{[WARNING] [17{X}] 'If' with a constant value (variable 'tmp' is equals 'false')!}
#
[WARNING] [23{X}] Possible NULL-dereference error (variable 'a' is NULL)!
          [{X}]     Variable 'a' becomes NULL-value
          [{X}]     Variable 'a' becomes NULL-value
:
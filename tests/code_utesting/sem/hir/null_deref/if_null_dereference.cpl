start() {
    ptr i32 a = 1;
    ptr i32 b = 1;
    if 1; {
        a = 0;
        return dref a;
    }

    return 0;
}

: OUTPUT
[WARNING] [4{X}] 'If' with a constant value 'true'!
[WARNING] [6{X}] NULL-dereference error (variable 'a' is NULL)!
:
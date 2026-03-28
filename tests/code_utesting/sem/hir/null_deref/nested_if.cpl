start() {
    ptr i32 a = 1;
    if 1; {
        if 2; {
            a = 0;
        }
        else a = 2;
    }
    dref a = 0;
}

:/ OUTPUT
[WARNING] [3{X}] 'If' with a constant value 'true'!
[WARNING] [4{X}] 'If' with a constant value 'true'!
[WARNING] [9{X}] Possible NULL-dereference error (variable 'a' is NULL)!
          [5{X}]     Variable 'a' becomes NULL-value
/:

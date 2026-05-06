start(i32 a) {
    ptr i0 p = 0x100;
    ptr i0 l = 0x100;
    ptr i0 b = 0x100;
    if 1; {
        p = 0;
    }
    if 0; {
        l = 0;
    }
    if a > 10; {
        b = 0;
    }
    dref p = 1;
    dref l = 1;
    dref b = 1;
}

:/ OUTPUT
[WARNING] [{X}:5:8] 'If' with a constant value 'true'!
[WARNING] [{X}:8:8] 'If' with a constant value 'false'!
[WARNING] [{X}:14:16] NULL-dereference error (variable 'p' is NULL)!
          [{X}:6:14]     Variable 'p' becomes NULL-value
[WARNING] [{X}:16:16] Possible NULL-dereference error (variable 'b' is NULL)!
          [{X}:12:14]     Variable 'b' becomes NULL-value
/:
start() {
    ptr i32 a = 1;
    @[straight]
    switch 1; {
        case 1; { a = 1; break; }
        case 2; { a = 0; }
        default { a = 0; }
    }
    dref a = 0;
}

:/ OUTPUT
[WARNING] [4:15] Condition with a constant value (variable 'tmp' is equals 'true' (1))!
          [4:15]     Variable 'tmp' declared as a constant here!
[WARNING] [4:15] Condition with a constant value (variable 'tmp' is equals 'false' (0))!
          [4:15]     Variable 'tmp' declared as a constant here!
[WARNING] [9:13] Possible NULL-dereference error (variable 'a' is NULL)!
          [7:23]     Variable 'a' becomes NULL-value
/:
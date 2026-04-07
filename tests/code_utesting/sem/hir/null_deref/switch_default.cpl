start() {
    ptr i32 a = 1;
    switch 1; {
        case 1; { a = 1; break; }
        case 2; { a = 0; }
        default { a = 0; }
    }
    dref a = 0;
}

:/ OUTPUT
[WARNING] [3:15] Condition with a constant value (variable 'tmp' is equals 'false' (0))!
          [3:15]     Variable 'tmp' declared as a constant here!
[WARNING] [3:15] Condition with a constant value (variable 'tmp' is equals 'false' (0))!
          [3:15]     Variable 'tmp' declared as a constant here!
[WARNING] [3:15] Condition with a constant value (variable 'tmp' is equals 'true' (1))!
          [3:15]     Variable 'tmp' declared as a constant here!
[WARNING] [3:15] Condition with a constant value (variable 'tmp' is equals 'false' (0))!
          [3:15]     Variable 'tmp' declared as a constant here!
[WARNING] [8:13] Possible NULL-dereference error (variable 'a' is NULL)!
          [6:23]     Variable 'a' becomes NULL-value
          [8:13]     Variable 'a' is assigned with the 'a' here
/:
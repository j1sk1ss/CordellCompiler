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
#
[WARNING] [3{X}] Condition with a constant value (variable 'tmp' is equals 'false' (0))!
          [3{X}]     Variable 'tmp' declared as a constant here!
[WARNING] [3{X}] Condition with a constant value (variable 'tmp' is equals 'false' (0))!
          [3{X}]     Variable 'tmp' declared as a constant here!
[WARNING] [3{X}] Condition with a constant value (variable 'tmp' is equals 'true' (1))!
          [3{X}]     Variable 'tmp' declared as a constant here!
[WARNING] [3{X}] Condition with a constant value (variable 'tmp' is equals 'false' (0))!
          [3{X}]     Variable 'tmp' declared as a constant here!
#
[WARNING] [{X}] Possible NULL-dereference error (variable 'a' is NULL)!
          [{X}]     Variable 'a' becomes NULL-value
/:

@[entry]
function foo() {
    i32 a = 1;
    if 1; {
        a = 0;
    }
    ptr i32 b = a as ptr i32;
    exit dref b;
}

:/ OUTPUT
[WARNING] [4{X}] 'If' with a constant value 'true'!
[WARNING] [8{X}] Possible NULL-dereference error (variable 'b' is NULL)!
          [5{X}]     Variable 'a' becomes NULL-value
          [7{X}]     Variable 'b' is assigned with the 'a' here
/:

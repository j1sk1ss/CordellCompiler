function foo(i32 a) -> i32 {
    return a - 10;
}

start() {
    ptr i32 a = 0xB8400;
    ptr i32 b = 0xB8400;
    @[no_fall]
    @[straight]
    switch 1; {
        case 1; { 
            a = foo(10);
            if 1; {
                b = a;
            }
        }
        case 2; { a = 1002; }
        default { a = 1111; }
    }

    exit dref b;
}

:/ OUTPUT
[WARNING] [13{X}] 'If' with a constant value 'true'!
[WARNING] [10{X}] Condition with a constant value (variable 'tmp' is equals 'true' (1))!
          [10{X}]     Variable 'tmp' declared as a constant here!
[WARNING] [10{X}] Condition with a constant value (variable 'tmp' is equals 'false' (0))!
          [10{X}]     Variable 'tmp' declared as a constant here!
[WARNING] [21{X}] Possible NULL-dereference error (variable 'b' is NULL)!
          [14{X}]     Variable 'b' becomes NULL-value
          [14{X}]     Variable 'b' is assigned with the 'a' here
          [12{X}]     Variable 'a' is assigned with NULL here
/:

@[entry]
function foo() {
    ptr i32 a = 1;
    ptr i32 b = 1;
    if 1; {
        @[no_fall]
        @[straight]
        switch 1; {
            case 1; { a = 0; }
            case 2; { a = 1; }
            default { a = 1; }
        }
        b = a;
    }
    else {
        a = 2;
    }
    ptr i32 c = b;
    dref c;
}

: OUTPUT
[WARNING] [5{X}] 'If' with a constant value 'true'!
[WARNING] [8{X}] Condition with a constant value (variable 'tmp' is equals 'true' (1))!
          [8{X}]     Variable 'tmp' declared as a constant here!
[WARNING] [8{X}] Condition with a constant value (variable 'tmp' is equals 'false' (0))!
          [8{X}]     Variable 'tmp' declared as a constant here!
[WARNING] [19{X}] Possible NULL-dereference error (variable 'c' is NULL)!
          [9{X}]     Variable 'a' becomes NULL-value
          [13{X}]     Variable 'b' is assigned with the 'a' here
          [18{X}]     Variable 'c' is assigned with the 'b' here
:
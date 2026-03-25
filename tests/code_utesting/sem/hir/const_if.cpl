function foo() {
    i32 a = 1;
    if a; {
        exit 1;
    }
    exit 2;
}

: OUTPUT
[WARNING] [3{X}] Condition with a constant value (variable 'a' is equals 'true' (1))!
          [3{X}]     Variable 'a' declared as a constant here!
:
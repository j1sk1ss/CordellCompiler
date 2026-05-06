function foo() {
    i32 a = 1;
    if a; {
        exit 1;
    }
    exit 2;
}

:/ OUTPUT
[WARNING] [{X}3:8] Condition with a constant value (variable 'a' is equals 'true' (1))!
          [{X}3:8]     Variable 'a' declared as a constant here!
/:

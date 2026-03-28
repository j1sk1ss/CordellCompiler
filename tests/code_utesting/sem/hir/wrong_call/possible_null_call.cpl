start() {
    ptr i0 a = 0xB800;
    if 1; {
        a = 0;
    }
    a();
}

: OUTPUT
[WARNING] [3{X}] 'If' with a constant value 'true'!
[WARNING] [6{X}] Possible NULL-dereference error (variable 'a' is NULL)!
          [4{X}]     Variable 'a' becomes NULL-value
          [4{X}]     Variable 'a' is assigned with NULL here
:
start() {
    ptr i32 a = 1;
    if 1; {
        if 2; a = 0;
        else a = 2;
        a = 1;
    }
    dref a = 0;
}

:/ OUTPUT
[WARNING] [{X}:3:8] 'If' with a constant value 'true'!
[WARNING] [{X}:4:12] 'If' with a constant value 'true'!
          [{X}:4:12]     Can't reach the 'else' branch! Consider to refactor the code.
/:
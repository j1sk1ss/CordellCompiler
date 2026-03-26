function foo() -> ptr i32 {
    return 0;
}

start() {
    ptr i32 a = foo();
    exit dref a;
}

: OUTPUT
[WARNING] [7{X}] NULL-dereference error (variable 'a' is NULL)!
          [6{X}]     Variable 'a' is assigned with NULL here
:
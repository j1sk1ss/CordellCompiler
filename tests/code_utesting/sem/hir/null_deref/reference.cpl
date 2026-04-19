start() {
    i32 a = 0;
    ptr i32 b = ref a;
    i32 c = dref b;
    dref c = 0;
}

:/ OUTPUT
[WARNING] [5:13] NULL-dereference error (variable 'c' is NULL)!
          [5:13]     Variable 'c' is assigned with NULL here
/:
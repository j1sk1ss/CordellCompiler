function foo() -> i0;
start() {
    i32 a = foo();
    exit 1;
}

:/ OUTPUT
[WARNING] [3:20] The 'foo' function doesn't return anything, but its result is being used! It will store a garbage in a variable.
3 | i32 a = foo();
  |         ^^^^^
[WARNING] [3:12] Function 'foo' has the 'i0' return type, and it doesn't match to the declaration type 'i32'!
3 | i32 a = foo();
  |         ^^^^^
/:
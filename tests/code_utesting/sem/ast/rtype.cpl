function foo() -> i32;
start() {
    u8 a = foo();
    exit 1;
}

:/ OUTPUT
[WARNING] [{X}3:11] Function 'foo' has the 'i32' return type, and it doesn't match to the declaration type 'u8'!
3 | u8 a = foo();
  |        ^^^^^
/:
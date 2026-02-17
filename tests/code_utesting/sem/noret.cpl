{
    function foo() -> i0;
    start() {
        i32 a = foo();
        exit 1;
    }
}

: OUTPUT
[WARNING] [4:24] The function='foo' doesn't return anything, but result is used!
4 | i32 a = foo();
  |         ^^^^^
[WARNING] [4:16] Function='foo' return type='i0' doesn't match to the declaration type='i32'!
4 | i32 a = foo();
  |         ^^^^^
:
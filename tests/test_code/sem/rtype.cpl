{
    function foo() => i32;
    start() {
        u8 a = foo();
        exit 1;
    }
}

: OUTPUT
[WARNING] [4:15] Function='foo' return type='i32' doesn't match to the declaration type='u8'!
4 | u8 a = foo();
  |        ^^^^^
:
{
    function foo(i8 a) -> i0;
    start() {
        foo(0xFFFF);
        i8 a = 0;
        foo(ref a);
        exit 1;
    }
}

: OUTPUT
[WARNING] [6:22] Illegal argument of 'i8' with 'a'! 'i8' can't handle bitness=64!
6 | foo(ref a);
  |     ^^^^^
[WARNING] [4:22] Illegal argument of 'i8' with '65535' (Number's bitness is=16, but 'i8' can handle bitness=8)!
4 | foo(65535);
  |     ^^^^^
:
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
[WARNING] [6:22] Illegal argument of 'i8' with 'a'! 'i8' can't handle bitness=max!
6 | foo(ref a);
  |     ^^^^^
[WARNING] [4:22] Illegal argument of 'i8' with '65535' (Number's bitness is=max/4, but 'i8' can handle bitness=max/8)!
4 | foo(65535);
  |     ^^^^^
:
start() {
    i8 a = 0xDEAD;
    i16 b = 123321123;
    i8 c = 0xF;
    exit 1;
}

:/ OUTPUT
[WARNING] [{X}3:26] Illegal declaration of 'b' with '123321123' (Number's bitness is 'max/2', but 'i16' can handle bitness 'max/4')!
3 | i16 b = 123321123;
  |         ^^^^^^^^^
[WARNING] [{X}2:22] Illegal declaration of 'a' with '57005' (Number's bitness is 'max/4', but 'i8' can handle bitness 'max/8')!
2 | i8 a = 57005;
  |        ^^^^^
/:
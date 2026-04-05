@[entry]
function foo(i32 a, i32 b) {
    return a - b;
}

:/ OUTPUT
BB1: fn _main(i32 a, i32 b)
%0 = %3;
%1 = &(*%4);
%5 = %0;
%5 = %5 - %1;
%2 = %5;
%6 = %2;
return %6;
BB2:
/:
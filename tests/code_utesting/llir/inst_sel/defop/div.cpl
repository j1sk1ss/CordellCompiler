@[entry]
function foo(i32 a, i32 b) {
    return a / b;
}

:/ OUTPUT
BB1: fn _main(i32 a, i32 b)
%0 = [rbp + 8];
%1 = &([rbp + 16]);
%3 = %0;
%4 = %1;
push(%5);
unknwn;
%6 = %3 / %4;
%5 = pop();
%2 = %6;
%7 = %2;
return %7;
BB2: fend
/:
@[entry]
function foo(i32 a, i32 b) {
    return a % b;
}

:/ OUTPUT
BB1: fn _main(i32 a, i32 b)
%0 = [rbp + 8];
%1 = &([rbp + 16]);
%3 = %0;
%4 = %1;
push(%5);
unknwn;
edx = %3 % %4;
eax = edx;
%5 = pop();
%2 = eax;
%6 = %2;
return %6;
BB2: fend
/:
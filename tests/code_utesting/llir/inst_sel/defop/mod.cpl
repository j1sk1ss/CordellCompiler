@[entry]
function foo(i32 a, i32 b) {
    return a % b;
}

:/ OUTPUT
BB1: fn _main(i32 a, i32 b)
%0 = [rbp + 8];
%1 = [rbp + 16];
rdx = rdx ^ rdx;
%3 = %0;
edx = %3 % %1;
%2 = edx;
%4 = %2;
return %4;
BB2:
/:
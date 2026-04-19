@[entry]
function foo(i32 a, i32 b) {
    return a % b;
}

:/ OUTPUT
BB1: fn _main(i32 a, i32 b)
%0 = %3;
%1 = &(*%4);
%5 = %0;
%7 = %1;
push(%8);
unknwn;
rdx = %6 % %7;
rax = rdx;
%8 = pop();
%2 = eax;
%9 = %2;
return %9;
BB2:
/:
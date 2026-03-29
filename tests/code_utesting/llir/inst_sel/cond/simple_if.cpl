@[entry]
function foo() {
    i32 a = 10 as i32;
    if a; {
        exit 1 as i8;
    }
}

:/ OUTPUT
BB1: fn _main()
%3 = $10;
%1 = %3;
%4 = %1;
%0 = %4;
cmp %0, 0;
je lb21;
jne lb19;
BB2: lb19:
%6 = $1;
%2 = %6;
%7 = %2;
%5 = %7;
exit %5;
BB3: lb21:
fend
/:
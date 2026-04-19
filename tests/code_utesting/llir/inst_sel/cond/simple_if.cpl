@[entry]
function foo() {
    i32 a = 10 as i32;
    if a; {
        exit 1 as i8;
    }
}

:/ OUTPUT
BB1: fn _main()
%1 = $10;
%0 = %1;
cmp %0, 0;
je lb21;
jne lb19;
BB2: lb19:
%2 = $1;
%3 = %2;
exit %3;
BB3: lb21:
fend
/:
@[entry]
function foo(i32 a) {
    if a == 1; {
        return 1;
    }
    else if a == 2; {
        return 2;
    }
    else {
        return 3;
    }
}

:/ OUTPUT
BB1: fn _main(i32 a)
%0 = %5;
%2 = $1;
%6 = %0;
cmp %6, %2;
sete al;
%1 = al;
cmp %1, 0;
je lb14;
jne lb13;
BB2: lb13:
%7 = $1;
return %7;
BB3: lb14:
%4 = $2;
%8 = %0;
cmp %8, %4;
sete al;
%3 = al;
cmp %3, 0;
je lb33;
jne lb32;
BB4: lb32:
%9 = $2;
return %9;
BB5: lb33:
%10 = $3;
return %10;
BB6: lb34:
BB7: lb15:
/:
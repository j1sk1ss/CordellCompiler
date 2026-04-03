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
%0 = [rbp + 8];
%2 = $1;
%5 = %0;
cmp %5, %2;
sete al;
%1 movzx al;
cmp %1, 0;
je lb14;
jne lb13;
BB2: lb13:
%6 = $1;
return %6;
BB3: lb14:
%4 = $2;
%7 = %0;
cmp %7, %4;
sete al;
%3 movzx al;
cmp %3, 0;
je lb33;
jne lb32;
BB4: lb32:
%8 = $2;
return %8;
BB5: lb33:
%9 = $3;
return %9;
BB6: lb34:
BB7: lb15:
/:
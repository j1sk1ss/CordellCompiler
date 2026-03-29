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
%6 = [rbp + 8];
%0 = %6;
%7 = $1;
%2 = %7;
%8 = %0;
%5 = %8;
cmp %5, %2;
sete al;
%1 movzx al;
cmp %1, 0;
je lb14;
jne lb13;
BB2: lb13:
%10 = $1;
%9 = %10;
return %9;
BB3: lb14:
%12 = $2;
%4 = %12;
%13 = %0;
%11 = %13;
cmp %11, %4;
sete al;
%3 movzx al;
cmp %3, 0;
je lb33;
jne lb32;
BB4: lb32:
%15 = $2;
%14 = %15;
return %14;
BB5: lb33:
%17 = $3;
%16 = %17;
return %16;
BB6: lb34:
BB7: lb15:
/:
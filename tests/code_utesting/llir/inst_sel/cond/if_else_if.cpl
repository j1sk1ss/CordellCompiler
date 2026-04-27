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
%1 = al;
cmp %1, 0;
je lb11;
jne lb10;
BB2: lb10:
%6 = $1;
return %6;
BB3: lb11:
%4 = $2;
%7 = %0;
cmp %7, %4;
sete al;
%3 = al;
cmp %3, 0;
je lb24;
jne lb23;
BB4: lb23:
%8 = $2;
return %8;
BB5: lb24:
%9 = $3;
return %9;
BB6: lb25:
jmp lb12;
BB7: lb12:
fend
/:
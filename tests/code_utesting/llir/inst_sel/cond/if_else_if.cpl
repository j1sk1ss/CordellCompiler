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
je lb12;
jne lb11;
BB2: lb11:
%6 = $1;
return %6;
BB3: lb12:
%4 = $2;
%7 = %0;
cmp %7, %4;
sete al;
%3 = al;
cmp %3, 0;
je lb25;
jne lb24;
BB4: lb24:
%8 = $2;
return %8;
BB5: lb25:
%9 = $3;
return %9;
BB6: lb26:
jmp lb13;
BB7: lb13:
fend
/:
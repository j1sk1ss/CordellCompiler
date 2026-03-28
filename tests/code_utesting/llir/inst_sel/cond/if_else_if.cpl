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
BB{X} fn _main(i32 a)
%6 = [rbp + 8];
%0 = %6;
%2 movsxd %0;
%7 = %2;
%5 = %7;
cmp %5, $1;
sete al;
%1 movzx al;
cmp %1, 0;
je lb{X}
jne lb{X}
BB{X} lb{X}
%9 = $1;
%8 = %9;
return %8;
BB{X} lb{X}
%4 movsxd %0;
%11 = %4;
%10 = %11;
cmp %10, $2;
sete al;
%3 movzx al;
cmp %3, 0;
je lb{X}
jne lb{X}
BB{X} lb{X}
%13 = $2;
%12 = %13;
return %12;
BB{X} lb{X}
%15 = $3;
%14 = %15;
return %14;
BB{X}
BB{X}
/:

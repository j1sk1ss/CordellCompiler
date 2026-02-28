{
    start(i32 a) {
        while a; {
            a += 1;
        }
    }
}

: OUTPUT
BB{X}: start
%3 = [rbp + 8];
%0 = %3;
BB{X}: lb{X}:
cmp %0, 0;
je lb{X};
jne lb{X};
BB{X}: lb{X}:
%5 = $1;
%2 = %5;
%6 = %0;
%4 = %6;
%4 = %4 + %2;
%7 = %4;
%1 = %7;
%8 = %1;
%0 = %8;
jmp lb{X};
BB{X}: lb{X}:
send
:
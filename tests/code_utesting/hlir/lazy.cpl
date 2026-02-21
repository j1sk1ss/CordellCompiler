{
    start() {
        i32 a;
        i32 b;
        i8 c = a && b;
        i8 d = a || b;
    }
}

: OUTPUT
BB{X}: start
cmp %0, 0;
je lb{X};
jne lb{X};

BB{X}: lb{X}:
%4 = %1;
jmp lb{X};

BB{X}: lb{X}:
%4 = 0;
jmp lb{X};

BB{X}: lb{X}:
%5 = %4 as i8;
%2 = %5;
cmp %0, 0;
je lb{X};
jne lb{X};

BB{X}: lb{X}:
%6 = %1;
jmp lb{X};

BB{X}: lb{X}:
%6 = 1;
jmp lb{X};

BB{X}: lb{X}:
%7 = %6 as i8;
%3 = %7;
send
:
{
    start() {
        i32 a;
        i32 b;
        i8 c = (a && b) as i8;
        i8 d = (a || b) as i8;
    }
}

:/ OUTPUT
BB1: start
cmp %0, 0;
je lb27;
jne lb26;
BB2: lb26:
%5 = %1;
jmp lb28;
BB3: lb27:
%5 = 0;
jmp lb28;
BB4: lb28:
%4 = %5 as i8;
%2 = %4;
cmp %0, 0;
je lb48;
jne lb47;
BB5: lb48:
%7 = %1;
jmp lb49;
BB6: lb47:
%7 = 1;
jmp lb49;
BB7: lb49:
%6 = %7 as i8;
%3 = %6;
send
/:

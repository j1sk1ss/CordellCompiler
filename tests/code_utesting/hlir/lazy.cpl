start() {
    i32 a;
    i32 b;
    i8 c = (a && b) as i8;
    i8 d = (a || b) as i8;
}

:/ OUTPUT
BB1: start
cmp %0, 0;
je lb16;
jne lb15;
BB2: lb15:
%6 = %1 as i64;
%5 = %6;
jmp lb17;
BB3: lb16:
%5 = 0;
jmp lb17;
BB4: lb17:
%4 = %5 as i8;
%2 = %4;
cmp %0, 0;
je lb30;
jne lb29;
BB5: lb30:
%9 = %1 as i64;
%8 = %9;
jmp lb31;
BB6: lb29:
%8 = 1;
jmp lb31;
BB7: lb31:
%7 = %8 as i8;
%3 = %7;
send
/:
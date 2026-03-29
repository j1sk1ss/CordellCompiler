start() {
    i32 a;
    i32 b;
    i8 c = (a && b) as i8;
    i8 d = (a || b) as i8;
}

:/ OUTPUT
BB1: start
cmp %0, 0;
je lb22;
jne lb21;
BB2: lb21:
%5 = %1;
jmp lb23;
BB3: lb22:
%5 = 0;
jmp lb23;
BB4: lb23:
%4 = %5 as i8;
%2 = %4;
cmp %0, 0;
je lb39;
jne lb38;
BB5: lb39:
%7 = %1;
jmp lb40;
BB6: lb38:
%7 = 1;
jmp lb40;
BB7: lb40:
%6 = %7 as i8;
%3 = %6;
send
/:
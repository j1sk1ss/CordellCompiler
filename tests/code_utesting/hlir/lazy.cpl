start() {
    i32 a;
    i32 b;
    i8 c = (a && b) as i8;
    i8 d = (a || b) as i8;
}

:/ OUTPUT
BB1: start
cmp %0, 0;
je lb15;
jne lb14;
BB2: lb14:
%5 = %1;
jmp lb16;
BB3: lb15:
%5 = 0;
jmp lb16;
BB4: lb16:
%4 = %5 as i8;
%2 = %4;
cmp %0, 0;
je lb26;
jne lb25;
BB5: lb26:
%7 = %1;
jmp lb27;
BB6: lb25:
%7 = 1;
jmp lb27;
BB7: lb27:
%6 = %7 as i8;
%3 = %6;
send
/:
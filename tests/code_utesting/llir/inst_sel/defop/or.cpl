start() {
    i8 a = (10 || 10) as i8;
}

:/ OUTPUT
BB1: start
%4 = $10;
%3 = %4;
cmp %3, 0;
je lb18;
jne lb17;
BB2: lb18:
%5 = $10;
%2 = %5;
jmp lb19;
BB3: lb17:
%6 = 1;
%2 = %6;
jmp lb19;
BB4: lb19:
%7 = %2;
%1 = %7;
%8 = %1;
%0 = %8;
send
/:
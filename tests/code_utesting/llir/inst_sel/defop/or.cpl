start() {
    i8 a = (10 || 10) as i8;
}

:/ OUTPUT
BB1: start
cmp $10, 0;
je lb18;
jne lb17;
BB2: lb18:
%3 = $10;
%2 = %3;
jmp lb19;
BB3: lb17:
%4 = 1;
%2 = %4;
jmp lb19;
BB4: lb19:
%5 = %2;
%1 = %5;
%6 = %1;
%0 = %6;
send
/:
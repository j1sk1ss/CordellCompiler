start() {
    i8 a = (10 && 10) as i8;
}

: OUTPUT
BB1: start
cmp $10, 0;
je lb21;
jne lb20;
BB2: lb20:
%3 = $10;
%2 = %3;
jmp lb22;
BB3: lb21:
%4 = 0;
%2 = %4;
jmp lb22;
BB4: lb22:
%5 = %2;
%1 = %5;
%6 = %1;
%0 = %6;
send
:
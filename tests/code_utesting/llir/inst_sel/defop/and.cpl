start() {
    i8 a = (10 && 10) as i8;
}

:/ OUTPUT
BB1: start
%4 = $10;
cmp %4, 0;
je lb12;
jne lb11;
BB2: lb11:
%3 = $10;
%2 = %3;
jmp lb13;
BB3: lb12:
%2 = 0;
jmp lb13;
BB4: lb13:
%1 = %2;
%0 = %1;
send
/:
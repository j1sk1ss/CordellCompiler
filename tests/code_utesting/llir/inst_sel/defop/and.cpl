start() {
    i8 a = (10 && 10) as i8;
}

:/ OUTPUT
BB1: start
%3 = $10;
cmp %3, 0;
je lb11;
jne lb10;
BB2: lb10:
%2 = $10;
jmp lb12;
BB3: lb11:
%2 = 0;
jmp lb12;
BB4: lb12:
%1 = %2;
%0 = %1;
send
/:
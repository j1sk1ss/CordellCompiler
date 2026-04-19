start() {
    i8 a = (10 && 10) as i8;
}

:/ OUTPUT
BB1: start
%3 = $10;
cmp %3, 0;
je lb18;
jne lb17;
BB2: lb17:
%2 = $10;
jmp lb19;
BB3: lb18:
%2 = 0;
jmp lb19;
BB4: lb19:
%1 = %2;
%0 = %1;
send
/:
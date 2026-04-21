start() {
    @[counter(10)] loop {
    }
}

:/ OUTPUT
BB1: start
%0 = 10;
BB2: lb9:
jmp lb10;
BB3: lb10:
%1 = %0 - 1;
%0 = %1;
cmp %0, 0;
je lb11;
jne lb9;
BB4: lb11:
send
/:
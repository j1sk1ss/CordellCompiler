start() {
    @[counter(10)] loop {
    }
}

:/ OUTPUT
BB1: start
%0 = 10;
BB2: lb12:
jmp lb13;
BB3: lb13:
%2 = %0;
%4 = %4 - 1;
%1 = %3;
%0 = %1;
cmp %0, 0;
je lb14;
jne lb12;
BB4:
BB5: lb14:
send
/:
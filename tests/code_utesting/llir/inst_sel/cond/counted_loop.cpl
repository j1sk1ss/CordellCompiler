start() {
    @[counter(10)] loop {
    }
}

: OUTPUT
BB{X} start
%2 = 10;
%0 = %2;
BB{X} lb{X}
jmp lb{X}
BB{X} lb{X}
%4 = %0;
%3 = %4;
%3 = %3 - 1;
%5 = %3;
%1 = %5;
%6 = %1;
%0 = %6;
cmp %1, 0;
je lb{X}
jne lb{X}
BB{X}
BB{X} lb{X}
send
:
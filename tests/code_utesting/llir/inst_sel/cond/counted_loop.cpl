{
    start() {
        @[counter(10)] loop {
        }
    }
}

: OUTPUT
BB{X}: start
%2 = 10;
%0 = %2;
BB{X}: lb6:
jmp lb7;
BB{X}: lb7:
%4 = %0;
%3 = %4;
%3 = %3 - 1;
%5 = %3;
%1 = %5;
%6 = %1;
%0 = %6;
cmp %1, 0;
je lb8;
jne lb6;
BB{X}:
BB{X}: lb8:
send
:
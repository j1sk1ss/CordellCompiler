start(i32 a) {
    while a; {
        a += 1 as i32;
    }
}

:/ OUTPUT
BB1: start
%3 = [rbp + 8];
%0 = %3;
BB2: lb13:
cmp %0, 0;
je lb15;
jne lb14;
BB3: lb14:
%5 = $1;
%1 = %5;
%6 = %0;
%4 = %6;
%4 = %4 + %1;
%7 = %4;
%2 = %7;
%8 = %2;
%0 = %8;
jmp lb13;
BB4: lb15:
send
/:

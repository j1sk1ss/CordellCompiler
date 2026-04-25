start() {
    i32 a = 1;
    i32 b = 0;
    @[counter(1000000)] loop {
        i32 tmp = a;
        a = a + b;
        b = tmp;
    }
    exit b;
}

:/ OUTPUT
BB1: start
BB6: %11 <<= 1000000;
%12 <<= $1;
%13 <<= $0;
BB2: lb19:
jmp lb20;
BB3: lb20:
%14 = %12;
%6 = %12 + %13;
%7 = %11 - 1;
%11 <<= %7;
%12 <<= %6;
%13 <<= %14;
cmp %7, 0;
je lb21;
jne lb19;
BB4: lb21:
exit %14;
BB5: send
/:
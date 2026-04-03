start() {
    i8 a = 1 as i8;
    i16 b = a;
    i32 c = b;
    i64 d = c;
}

:/ OUTPUT
BB1: start
%4 = $1;
%0 = %4;
%5 movsx %0;
%1 = %5;
%6 movsx %1;
%2 = %6;
%7 movsxd %2;
%3 = %7;
send
/:
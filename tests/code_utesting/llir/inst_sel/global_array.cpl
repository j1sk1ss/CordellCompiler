glob arr a[10, i32];
start() {
    a[0] = 1 as i32;
}

:/ OUTPUT
%0 = arr_alloc($10);
BB1: start
%1 = $1;
%2 = &(%0);
%6 = $0;
%6 = %6 * 4;
%3 movsx %6;
%5 = %3;
%7 = %2;
%7 = %7 + %5;
%4 = %7;
*(%4) = %1;
send
/:
glob arr a[10, i32];
start() {
    a[0] = 1 as i32;
}

: OUTPUT
%0 = arr_alloc($10);
BB1: start
%1 = $1 as i32;
%2 = &(%0);
%3 = $0 * 4;
%5 = %3 as u64;
%4 = %2 + %5;
*(%4) = %1;
send
:
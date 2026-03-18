glob arr a[10, i32];
start() {
    a[0] = 1;
}

: OUTPUT
%0 = arr_alloc($10);
BB{X} start
%1 = &(%0);
%2 = $0 * 4;
%4 = %2 as u64;
%3 = %1 + %4;
%5 = $1 as i32;
*(%3) = %5;
send
:
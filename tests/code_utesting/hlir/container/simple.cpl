container node {
    i32 a;
    i32 b;
}

start() {
    ptr node a;
    node b;

    a = ref b;

    a.a = 10;
    a.b = 10;
}

:/ OUTPUT
BB1: start
%3 = arr_alloc(16);
%4 = &(%3);
%2 = %4;
%5 = %2 + 0;
%6 = $10 as i32;
*(%5) = %6;
%7 = %2 + 8;
%8 = $10 as i32;
*(%7) = %8;
send
/:
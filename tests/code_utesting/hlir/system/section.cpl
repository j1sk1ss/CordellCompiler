@[section(".text")] function foo() {
    i32 a = 1 as i32;
    return a;
}

start() {
    exit foo();
}

: OUTPUT
BB2: fn foo0()
%1 = $1 as i32;
%0 = %1;
return %0;
BB3:
BB4: start
call foo0();
%2 = fret();
exit %2;
BB5:
:
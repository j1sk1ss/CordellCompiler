@[section(".text")] function foo() {
    i32 a = 1 as i32;
    return a;
}

start() {
    exit foo() as u8;
}

:/ OUTPUT
BB2: fn foo0()
%1 = $1 as i32;
%0 = %1;
return %0;
BB3: fend
BB4: start
call foo0;
%3 = fret();
%2 = %3 as u8;
exit %2;
BB5: send
/:
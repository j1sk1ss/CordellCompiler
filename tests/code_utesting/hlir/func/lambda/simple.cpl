start() {
    ptr i0 f = (i32 a) => { return a * a; };
    exit f(10) as u8;
}

:/ OUTPUT
BB2: start
jmp lb8;
BB3: lb8:
%3 = &(__anon_function_lambda1(i32 a));
%0 = %3;
stparam($10);
call %0;
%5 = fret();
%4 = %5 as u8;
exit %4;
BB4: send
BB5: fn __anon_function_lambda1(i32 a)
%1 = ldparam();
%2 = %1 * %1;
return %2;
BB6: fend
/:
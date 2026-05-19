start() {
    exit ((i32 a) => { return a * a; })(10) as u8;
}

:/ OUTPUT
BB2: start
jmp lb8;
BB3: lb8:
%3 = &(__anon_function_lambda1(i32 a));
stparam($10);
call %3;
%4 = fret();
%1 = %4 as u8;
exit %1;
BB4: send
BB5: fn __anon_function_lambda1(i32 a)
%0 = ldparam();
%2 = %0 * %0;
return %2;
BB6: fend
/:
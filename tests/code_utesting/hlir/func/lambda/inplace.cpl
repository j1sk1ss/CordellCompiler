start() {
    exit ((i32 a) => { return a * a; })(10);
}

:/ OUTPUT
BB2: start
jmp lb12;
BB3: lb12:
%2 = &(__anon_function_lambda1(i32 a));
stparam($10);
call %2;
%3 = fret();
exit %3;
BB4:
BB5: fn __anon_function_lambda1(i32 a)
%0 = ldparam();
%1 = %0 * %0;
return %1;
BB6:
/:
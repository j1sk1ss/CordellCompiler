start() {
    ptr i0 getter = () => 10;
    exit getter() as i8;
}

:/ OUTPUT
BB2: start
jmp lb8;
BB3: lb8:
%1 = &(__anon_function_lambda1());
%0 = %1;
call %0;
%3 = fret();
%2 = %3 as i8;
%4 = %2 as u8;
exit %4;
BB4: send
BB5: fn __anon_function_lambda1()
return $10;
/:
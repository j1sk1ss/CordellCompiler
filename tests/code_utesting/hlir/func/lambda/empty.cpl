start() {
    i32 a = (() => { return 10; })() as i32;
}

:/ OUTPUT
BB2: start
jmp lb16;
BB3: lb16:
%2 = &(__anon_function_lambda1());
call %2;
%3 = fret();
%1 = %3 as i32;
%0 = %1;
send
BB4: fn __anon_function_lambda1()
return $10;
BB5:
/:
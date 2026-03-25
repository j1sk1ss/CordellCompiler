start() {
    i32 a = (() => { return 10; })();
}

: OUTPUT
BB{X} start
jmp lb16;
BB{X} lb{X}
%1 = &(__anon_function_lambda0());
call %1;
%2 = fret();
%3 = %2 as i32;
%0 = %3;
send
BB{X} fn __anon_function_lambda0()
return $10;
BB{X}
:
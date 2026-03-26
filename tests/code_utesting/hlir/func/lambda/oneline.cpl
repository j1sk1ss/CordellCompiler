start() {
    ptr i0 getter = () => 10;
    exit getter();
}

: OUTPUT
BB{X} start
jmp lb14;
BB{X} lb{X}
%1 = &(__anon_function_lambda1());
%0 = %1;
call %0;
%2 = fret();
exit %2;
BB{X}
BB{X} fn __anon_function_lambda1()
return $10;
fend
:
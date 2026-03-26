start() {
    exit ((i32 a) => { return a * a; })(10);
}

: OUTPUT
BB{X} start
jmp lb14;
BB{X} lb{X}
%2 = &(__anon_function_lambda1(i32 a));
stparam($10);
call %2;
%3 = fret();
exit %3;
BB{X}
BB{X} fn __anon_function_lambda1(i32 a)
%0 = ldparam();
%1 = %0 * %0;
return %1;
BB{X}
:
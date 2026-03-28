start() {
    ptr i0 f = (i32 a) => { return a * a; };
    exit f(10);
}

:/ OUTPUT
BB{X} start
jmp lb14;
BB{X} lb{X}
%3 = &(__anon_function_lambda1(i32 a));
%0 = %3;
stparam($10);
call %0;
%4 = fret();
exit %4;
BB{X}
BB{X} fn __anon_function_lambda1(i32 a)
%1 = ldparam();
%2 = %1 * %1;
return %2;
BB{X}
/:

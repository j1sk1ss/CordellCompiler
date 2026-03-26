start() {
    exit ((i32 a) => {
        while a; {
            a -= 1;
        }
        return 0;
    })(10);
}

: OUTPUT
BB{X} start
jmp lb14;
BB{X} lb{X}
%3 = &(__anon_function_lambda1(i32 a));
stparam($10);
call %3;
%4 = fret();
exit %4;
BB{X}
BB{X} fn __anon_function_lambda1(i32 a)
%0 = ldparam();
BB{X} lb{X}
cmp %0, 0;
je lb26;
jne lb25;
BB{X} lb{X}
%2 = $1 as i32;
%1 = %0 - %2;
%0 = %1;
jmp lb24;
BB{X} lb{X}
return $0;
BB{X}
:
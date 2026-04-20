start() {
    exit ((i32 a) => {
        while a; {
            a -= 1 as i32;
        }
        return 0;
    })(10) as i8;
}

:/ OUTPUT
BB2: start
jmp lb10;
BB3: lb10:
%4 = &(__anon_function_lambda1(i32 a));
stparam($10);
call %4;
%5 = fret();
%1 = %5 as i8;
exit %1;
BB4:
BB5: fn __anon_function_lambda1(i32 a)
%0 = ldparam();
BB6: lb18:
cmp %0, 0;
je lb20;
jne lb19;
BB7: lb19:
%2 = $1 as i32;
%3 = %0 - %2;
%0 = %3;
jmp lb18;
BB8: lb20:
return $0;
BB9:
/:
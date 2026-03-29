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
jmp lb14;
BB3: lb14:
%4 = &(__anon_function_lambda1(i32 a));
stparam($10);
call %4;
%5 = fret();
%1 = %5 as i8;
exit %1;
BB4:
BB5: fn __anon_function_lambda1(i32 a)
%0 = ldparam();
BB6: lb24:
cmp %0, 0;
je lb26;
jne lb25;
BB7: lb25:
%2 = $1 as i32;
%3 = %0 - %2;
%0 = %3;
jmp lb24;
BB8: lb26:
return $0;
BB9:
/:
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
jmp lb17;
BB3: lb17:
%4 = &(__anon_function_lambda1(i32 a));
stparam($10);
call %4;
%5 = fret();
%1 = %5 as i8;
exit %1;
BB4:
BB5: fn __anon_function_lambda1(i32 a)
%0 = ldparam();
BB6: lb27:
cmp %0, 0;
je lb29;
jne lb28;
BB7: lb28:
%2 = $1 as i32;
%3 = %0 - %2;
%0 = %3;
jmp lb27;
BB8: lb29:
return $0;
BB9:
/:

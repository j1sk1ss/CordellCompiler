start() {
    function get(i32 a) -> i32 {
        return a * 10;
    }
    exit get(10);
}

:/ OUTPUT
BB2: start
jmp lb10;
BB3: lb10:
stparam($10);
call get1(i32 a) -> i32;
%3 = fret();
exit %3;
BB4:
BB5: fn get1(i32 a) -> i32
%0 = ldparam();
%2 = $10 as i32;
%1 = %0 * %2;
return %1;
BB6:
/:
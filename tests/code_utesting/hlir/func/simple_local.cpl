start() {
    function get(i32 a) -> i32 {
        return a * 10;
    }
    exit get(10) as u8;
}

:/ OUTPUT
BB2: start
jmp lb7;
BB3: lb7:
stparam($10);
call get1;
%4 = fret();
%3 = %4 as u8;
exit %3;
BB4: send
BB5: fn get1(i32 a) -> i32
%0 = ldparam();
%2 = $10 as i32;
%1 = %0 * %2;
return %1;
BB6: fend
/:
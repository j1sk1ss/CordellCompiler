start() {
    function get(i32 a) -> i32 {
        return a * 10;
    }
    exit get(10);
}

:/ OUTPUT
BB{X} start
jmp lb10;
BB{X} lb{X}
stparam($10);
call get1(i32 a) -> i32;
%3 = fret();
exit %3;
BB{X}
BB{X} fn get1(i32 a) -> i32
%0 = ldparam();
%2 = %0 as i64;
%1 = %2 * $10;
return %1;
BB{X}
/:

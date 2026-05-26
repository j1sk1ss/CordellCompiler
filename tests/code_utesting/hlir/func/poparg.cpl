{
    @[entry]
    function foo(...) {
        @[poparg] i64 a;
        @[poparg] ptr f32 b;
    }
}

:/ OUTPUT
BB1: fn _main(...)
unknwn;
%0 = *(%2);
%3 = %2 + 8;
%2 = %3;
%1 = *(%2);
%4 = %2 + 8;
%2 = %4;
fend
/:
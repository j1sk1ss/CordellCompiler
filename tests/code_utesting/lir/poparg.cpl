{
    function foo(...) {
        i64 a = poparg as i64;
        ptr f32 b = poparg as ptr f32;
    }
}

: OUTPUT
BB{X}: fn foo0(...)
%3 = ldparam();
%2 = %3 as i64;
%0 = %2;
%5 = ldparam();
%4 = %5 as f32;
%1 = %4;
fend
:
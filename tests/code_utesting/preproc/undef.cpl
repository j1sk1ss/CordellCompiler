{
    #define ASD 1
    function foo(i32 a = ASD);
    #undef ASD
    function bar(i32 a = ASD);
    : #undef ASD
    #undef ASD :
    function baz(u32 a = ASD);
    function main(u32 a = ASD);
}

: OUTPUT
{
    function foo(i32 a = 1);
    function bar(i32 a = ASD);
    function baz(u32 a = ASD);
    function main(u32 a = ASD);
}
:
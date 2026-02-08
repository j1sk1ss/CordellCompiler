{
    #define A 1
    #ifdef A
    function a();
    #define B 1
    #ifdef B
    function b();
    #undef B
    #endif
    #ifdef B
    function c();
    #endif
    function d();
    #undef A
    #endif
}

: OUTPUT
{
    function a();
    function b();
    function d();
}
:
{
    function foo() {
        return;
    } : -> void :

    function bar() {
        return;
    } : -> void :
    
    start(i64 argc, ptr u64 argv) {
        bar();
        exit 0;
    }
}

: Expected
{
    function bar() {
        return;
    }
    
    start(i64 argc, ptr u64 argv) {
        bar();
        exit 0;
    }
}
:

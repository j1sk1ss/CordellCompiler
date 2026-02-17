{
    glob function foo() -> i32;
    start() {
        glob i32 a;
        glob ro i32 b;
        ro glob i32 c;
        ro i32 d;
        ro glob ptr i32 e;
    }
}

: OUTPUT
BB{X}: start
send
:
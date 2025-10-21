{
    function foo() => i64 { return 0; }
    start(i64 argc, ptr u64 argv) {
        i32 a = 0xFFFF;
        i8 b = a;     : <= Will produce a warning :

        i8 c = foo(); : <= Will produce a warning :

        i8 d = 0xFFF; : <= Will produce a warning :
        u8 f = -1;    : <= Will produce a warning :
        exit 0;
    }
}
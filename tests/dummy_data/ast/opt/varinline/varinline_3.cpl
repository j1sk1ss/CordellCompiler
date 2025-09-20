{
    extern i8 exval;
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        i32 b = a; 

        a = 12;
        i32 c = a;

        a = 13;
        i32 d = a;

        if exval; {
            a = 15;
            i32 b2 = a;

            a = exval;
            i32 b3 = a;
        }
        else {
            a = 14;
            i32 b1 = a;

            a = exval;
            i32 b3 = a;
        }

        i32 d1 = a;
        exit a;
    }
}

: Expected
{
    extern i8 exval;
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        a = 13;

        if exval; {
            a = exval;
        }
        else {
            a = exval;
        }

        exit a;
    }
}
:

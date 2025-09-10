{
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        {
            i64 b = a;
        }

        if a; {
            i8 b  = 0;
            i8 b1 = b;
        }

        exit a;
    }
}

: Expected
{
    start(i64 argc, ptr u64 argv) {
        if 10; {
        }

        exit 10;
    }
}
:

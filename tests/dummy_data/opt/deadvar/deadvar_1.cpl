{
    start(i64 argc, ptr u64 argv) {
        i32 a = 0;
        while a; {
            a = a + 1;
        }

        i16 b = 10;
        while b; {
            b = b + 1;
        }

        i64 c = b;
        exit c;
    }
}

: Expected
{
    start(i64 argc, ptr u64 argv) {
        i16 b = 10;
        while b; {
            b = b + 1;
        }

        i64 c = b;
        exit c;
    }
}
:

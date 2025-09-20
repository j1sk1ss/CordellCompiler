{
    start(i64 argc, ptr u64 argv) {
        i8 a = 10;
        i8 c = a;

        a = 15;
        i8 d = a;

        while a < 100; {
            a = a + 1;
        }

        exit d;
    }
}

: Expected
{
    start(i64 argc, ptr u64 argv) {
        i8 a = 10;
        a = 15;
        while a < 100; {
            a = a + 1;
        }

        exit 15;
    }
}
:

{
    start(i64 argc, ptr u64 argv) {
        i8 a = 10;

        while a < 100; {
            a = a + 1;
        }

        a = 12;
        exit a;
    }
}

: Expected
{
    start(i64 argc, ptr u64 argv) {
        i8 a = 10;

        while a < 100; {
            a = a + 1;
        }

        a = 12;
        exit 12;
    }
}
:

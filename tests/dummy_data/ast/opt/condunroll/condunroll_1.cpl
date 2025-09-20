{
    start() {
        if 1; {
            i32 a = 10;
            a = a + 20;
        }
        else {
            i32 b = 20;
            i32 c = b + 10;
        }

        exit 0;
    }
}

: Expected
{
    start() {
        {
            i32 a = 10;
            a = a + 20;
        }

        exit 0;
    }
}
:

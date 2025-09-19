{
    {
        start(i64 argc, ptr u64 argv) {
            i64 a = 10;

            {
                i16 b = 10;
                b = b + b;

                {
                    i64 c = 10;
                    c = b;
                }
            }

            exit a;
        }
    }
}

: Expected
{
    {
        start(i64 argc, ptr u64 argv) {
            i64 a = 10;
            exit a;
        }
    }
}
:
{
    start(i64 argc, ptr u64 argv) {
        switch 1; {
            case 1; {
                i32 a = 10;
                a = a + 1;
            }
            default {
                i32 b = 10;
                b = b + 1;
            }
        }

        i8 c = 0;
        switch 1; {
            case 2; {
            }
            default {
                c = 1;
            }
        }

        exit c;
    }
}

: Exprected
{
    start(i64 argc, ptr u64 argv) {
        i8 c = 0;
        switch 1; {
            default {
                c = 1;
            }
        }

        exit c;
    }
}
:

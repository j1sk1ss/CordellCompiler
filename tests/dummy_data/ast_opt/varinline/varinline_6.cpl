{
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;

        a = 11;
        i32 c = a;
        switch a; {
            case 1; {
                i8 b = a;
            }
            case 2; {
                i8 b = a;
            }
            case 3; {
                i8 b = a;
            }
            default {
                i8 b = a;
            }
        }

        a = 12;
        exit a;
    }
}

: Expected
{
    start(i64 argc, ptr u64 argv) {
        switch 11; {
            case 1; {
            }
            case 2; {
            }
            case 3; {
            }
            default {
            }
        }

        exit 12;
    }
}
:

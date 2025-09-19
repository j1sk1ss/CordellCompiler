{
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        switch a; {
            case 1; {
                a = 15;
                i8 b = a;
            }
            case 2; {
                a = 20;
                i8 b = a;
            }
            case 3; {
                a = 25;
                i8 b = a;
            }
            default {
                a = 30;
                i8 b = a;
            }
        }

        exit a;
    }
}

: Expected
{
    start(i64 argc, ptr u64 argv) {
        i32 a = 10;
        switch 10; {
            case 1; {
                a = 15;
            }
            case 2; {
                a = 20;
            }
            case 3; {
                a = 25;
            }
            default {
                a = 30;
            }
        }

        exit a;
    }
}
:

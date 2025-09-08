{
    function foo() {
        return 0;
    }

    start() {
        i32 a = 0;
        i32 b = 1;
        i32 c = 2;

        { : Should be deleted :
            i16 d = 10 + a;
            d = d + 123;
        }

        { : Shouldnt be deleted :
            i16 d = 20 + c;
            c = d + 10;
        }

        if (a + 1) > 10; { : Should be deleted :
            i16 k = 12;
            k = k + 1;
        }

        if a; { : Shouldnt be deleted :
            a = a + 1;
        }

        switch c + a; {
            case c; { : Should be deleted :
                i16 kk = 10;
                kk = kk + 10;
            }
            default { : Shouldnt be deleted :
                i16 dd = 12;
                foo();
            }
        }

        { : Shouldnt be deleted :
            foo();
        }

        exit 0;
    }
}
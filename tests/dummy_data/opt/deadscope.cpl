{
    function foo() {
        return 0;
    }

    start() {
        int a = 0;
        int b = 1;
        int c = 2;

        { : Should be deleted :
            short d = 10 + a;
            d = d + 123;
        }

        { : Shouldnt be deleted :
            short d = 20 + c;
            c = d + 10;
        }

        if (a + 1) > 10; { : Should be deleted :
            short k = 12;
            k = k + 1;
        }

        if a; { : Shouldnt be deleted :
            a = a + 1;
        }

        switch c + a; {
            case c; { : Should be deleted :
                short kk = 10;
                kk = kk + 10;
            }
            default { : Shouldnt be deleted :
                short dd = 12;
                foo();
            }
        }

        { : Shouldnt be deleted :
            foo();
        }

        exit 0;
    }
}
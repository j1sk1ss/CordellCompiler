{
    start() {
        i32 a = 5;
        while a > 0; {
            a = a - 1;
        }

        i32 c = 0;
        while 0; {
            c = c + 1;
        }
        else {
            c = 100;
        }
        
        exit c;
    }
}

: Expected
{
    start() {
        i32 a = 5;
        while a > 0; {
            a = a - 1;
        }

        i32 c = 0;
        {
            c = 100;
        }
        
        exit c;
    }
}
:
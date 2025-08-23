{
    start {
        if 1; {
            int a = 10;
            a = a + 20;
        }
        else {
            int b = 20;
            int c = b + 10;
        }

        int a = 5;
        while a > 0; {
            a = a - 1;
        }

        int c = 0;
        while 0; {
            c = c + 1;
        }
        else {
            c = 100;
        }

        switch 10; {
            case 10; {
                int b = 10;
                b = b + 10;
            }
            case 11; {
                int d = 10;
                d = d + 10;
            }
            default {
                int k = 10;
                k = k + 10;
            }
        }
    } exit c;
}
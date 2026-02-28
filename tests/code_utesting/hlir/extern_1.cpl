{
    extern function printf(ptr i8 fmt, ...) -> i0;
    start() {
        printf("Hello world! %i", 10);
    }
}

: OUTPUT
(fun) extern 0;
BB{X}: start
use str(Hello world! %i);
use $10;
stparam(str(Hello world! %i));
stparam($10);
call printf0(i8* fmt, ...) -> i0;
send
:
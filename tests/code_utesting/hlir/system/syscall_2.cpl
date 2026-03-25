start() {
    syscall(syscall(1));
}

: OUTPUT
BB{X} start
stparam($1);
syscall;
%0 = fret();
use %0;
stparam(%0);
syscall;
send
:
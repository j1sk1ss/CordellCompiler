{
    start() {
        syscall(1, 2, 3, 4);
    }
}

: OUTPUT
BB{X}: start
use $1;
use $2;
use $3;
use $4;
stparam($1);
stparam($2);
stparam($3);
stparam($4);
syscall;
send
:
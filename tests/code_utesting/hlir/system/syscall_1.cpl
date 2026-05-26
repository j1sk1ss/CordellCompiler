start() {
    syscall(1, 2, 3, 4);
}

:/ OUTPUT
BB1: start
stparam($4);
stparam($3);
stparam($2);
stparam($1);
syscall;
send
/:
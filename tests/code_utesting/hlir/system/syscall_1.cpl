start() {
    syscall(1, 2, 3, 4);
}

:/ OUTPUT
BB{X} start
stparam($1);
stparam($2);
stparam($3);
stparam($4);
syscall;
send
/:

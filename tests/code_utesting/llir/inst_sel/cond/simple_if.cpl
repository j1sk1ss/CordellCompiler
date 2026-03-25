@[entry]
function foo() {
    i32 a = 10;
    if a; {
        exit 1;
    }
}

: OUTPUT
BB{X} fn _main()
%2 = $10;
%1 = %2;
%3 = %1;
%0 = %3;
cmp %0, 0;
je lb{X}
jne lb{X}
BB{X} lb{X}
%5 = $1;
%4 = %5;
exit %4;
BB{X} lb{X}
fend
:
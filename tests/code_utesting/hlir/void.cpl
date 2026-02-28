{
    start() {
        ptr i0 a = 0x1;
        a = 0x2;
    }
}

: OUTPUT
BB{X}: start
%1 = $1 as u64;
%0 = %1;
%2 = $2 as u64;
%0 = %2;
send
:
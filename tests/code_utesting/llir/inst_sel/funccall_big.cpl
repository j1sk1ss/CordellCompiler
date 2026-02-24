{
    function foo(...);
    start() {
        foo(1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11);
    }
}

: OUTPUT
BB{X}: start
use $1;
use $2;
use $3;
use $4;
use $5;
use $6;
use $7;
use $8;
use $9;
use $10;
use $11;
%6 = $1;
%0 <<= %6;
%7 = $2;
%1 <<= %7;
%8 = $3;
%2 <<= %8;
%9 = $4;
%3 <<= %9;
%10 = $5;
%4 <<= %10;
%11 = $6;
%5 <<= %11;
push($7);
push($8);
push($9);
push($10);
push($11);
call foo0(...);
send
:
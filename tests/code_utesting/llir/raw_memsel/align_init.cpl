start() {
    i32 a = 1 as i32;
    @[align(64)] i32 b = 1 as i32;
}

:/ OUTPUT
BB1: start
[rbp - 8] = $1;
%4 = [rbp - 8];
[rbp - 16] = %4;
[rbp - 24] = $1;
%5 = [rbp - 24];
[rbp - 88] = %5;
send
/:
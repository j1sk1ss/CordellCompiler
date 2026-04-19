@[entry]
function main() {
    @[align(128)] i32 a = 1 as i32;
    @[align(128)] i32 b = a;
}

:/ OUTPUT
BB1: fn _main()
[rbp - 8] = $1;
%3 = [rbp - 8];
[rbp - 136] = %3;
%4 = [rbp - 136];
[rbp - 264] = %4;
fend
/:
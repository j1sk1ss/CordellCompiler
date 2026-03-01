{
    @[entry]
    function main() {
        @[align(128)] i32 a = 1;
        @[align(128)] i32 b = a;
    }
}

: OUTPUT
BB{X}: fn _main0()
[rbp - 8] = $1;
[rbp - 16] = [rbp - 8];
[rbp - 24] = [rbp - 16];
[rbp - 152] = [rbp - 24];
[rbp - 280] = [rbp - 152];
[rbp - 408] = [rbp - 280];
fend
:
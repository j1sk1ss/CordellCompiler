@[entry]
function main() {
    @[align(128)] i32 a = 1;
    @[align(128)] i32 b = a;
}

: OUTPUT
BB{X} fn _main()
r15 = $1;
[rbp - 8] = r15;
r15 = [rbp - 8];
[rbp - 136] = r15;
r15 = [rbp - 136];
[rbp - 264] = r15;
fend
:
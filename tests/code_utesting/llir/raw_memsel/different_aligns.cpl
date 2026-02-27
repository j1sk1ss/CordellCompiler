{
    start() {
        align(16) i32 a = 0;
        align(64) {
            i32 b = 0;
            i32 c = 0;
        }
        align(8) i32 d = 0;
    }
}

: OUTPUT
BB{X}: start
[rbp - 8] = $0;
[rbp - 16] = [rbp - 8];
[rbp - 24] = [rbp - 16];
[rbp - 40] = [rbp - 24];
[rbp - 48] = $0;
[rbp - 56] = [rbp - 48];
[rbp - 64] = [rbp - 56];
[rbp - 128] = [rbp - 64];
[rbp - 136] = $0;
[rbp - 144] = [rbp - 136];
[rbp - 152] = [rbp - 144];
[rbp - 216] = [rbp - 152];
[rbp - 224] = $0;
[rbp - 232] = [rbp - 224];
[rbp - 240] = [rbp - 232];
[rbp - 248] = [rbp - 240];
send
:
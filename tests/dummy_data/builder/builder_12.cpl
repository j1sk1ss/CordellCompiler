{
    function strlen(ptr i8 string) {
        i64 s = 0;
        while dref string; {
            string = string + 1;
            s = s + 1;
        }

        return s;
    }

    function puts(ptr i8 string) {
        i64 size = strlen(string);
        return syscall(1, 1, string, size);
    }

    start(i64 argc, ptr u64 argv) {
        i32 n = 10;
        arr data[n, i8];

        i32 i = n - 1;
        while i > 0; {
            data[i] = 0;
            i -= 1;
        }

        data[0] = 'H';
        puts(data);
        exit 0;
    }
}
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
        return syscall(1, 1, string, strlen(string));
    }

    start(i64 argc, ptr i64 argv) {
        puts("Hello, World!\n");
        exit 0;
    }
}
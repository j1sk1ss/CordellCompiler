{
    function strlen(ptr i8 string) {
        i64 s = 0;
        while dref string; {
            string = string + 1;
            s = s + 1;
        }

        return s;
    } : -> int :

    function puts(ptr i8 string) {
        return syscall(1, 1, string, strlen(string));
    } : -> int :

    start(i64 argc, ptr i64 argv) {
        puts(argv[1]);
        exit 0;
    }
}
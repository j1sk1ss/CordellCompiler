{
    function strlen(ptr i8 string) {
        i64 s = 0;
        while dref string; {
            string = string + 1;
            s = s + 1;
        }

        return s;
    } : -> i32 :

    function puts(ptr i8 string) {
        return syscall(1, 1, string, strlen(string));
    } : -> i32 :

    start(i64 argc, ptr i64 argv) {
        puts("Hello, World!\n");
        exit 0;
    }
}
{
    function strlen(ptr char string) {
        long s = 0;
        while dref string; {
            string = string + 1;
            s = s + 1;
        }

        return s;
    } : -> int :

    function puts(ptr char string) {
        long size = strlen(string);
        return syscall(1, 1, string, size);
    } : -> int :

    start(long argc, ptr long argv) {
        puts("Hello, World!");
        exit 0;
    }
}
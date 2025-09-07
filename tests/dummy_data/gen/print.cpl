{
    function strlen(ptr char string) {
        int length = 0;
        while dref string; {
            length = length + 1;
            string = string + 1;
        }

        return length;
    }

    function puts(ptr char string) {
        long strSize = strlen(string);
        return syscall(1, 1, string, strSize);
    }

    start() {
        puts("Hello there!");
        exit 0;
    }
}
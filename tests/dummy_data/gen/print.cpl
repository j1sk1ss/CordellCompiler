{
    function strlen(ptr i8 string) {
        i32 length = 0;
        while dref string; {
            length = length + 1;
            string = string + 1;
        }

        return length;
    }

    function puts(ptr i8 string) {
        return syscall(1, 1, string, strlen(string));
    }

    start() {
        puts("Hello there!");
        exit 0;
    }
}
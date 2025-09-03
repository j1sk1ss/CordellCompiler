{
    function strlen(ptr char string) {
        long s = 0;
        while string[0]; {
            string = string + 1;
            s = s + 1;
        }

        return s;
    }

    function puts(ptr char string) {
        long size = strlen(string);
        return syscall(1, 1, string, size);
    }

    start(long argc, ptr long argv) {
        puts(argv[1]);
        exit 0;
    }
}
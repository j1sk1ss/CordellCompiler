{
    function atoi(ptr i8 buffer, i32 bsize) => i32 {
        i32 num = 0;
        i32 index = 0;
        while index < bsize; {
            num *= 10;
            i8 sym = buffer[index];
            num += sym - '0';
            index += 1;
        }

        return num;
    }

    start(i64 argc, ptr u64 argv) {
        exit atoi("123", 3);
    }
}